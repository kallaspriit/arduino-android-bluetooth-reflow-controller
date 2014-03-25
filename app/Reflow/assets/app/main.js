if (!window.native) {
	window.native = {
		getPairedDevices: function() {
			return JSON.stringify(['Dummy']);
		},
		sendMessage: function(message) {
			console.log('send message', message);
		}
	}
}

var Intent = {
		NONE: 0,
		MAIN_MENU: 1,
		START_REFLOW: 2,
		CONFIGURE_PID: 3
	},
	connectedDeviceName = null,
	currentBluetoothState = null,
	lastBluetoothState = null,
	currentApplicationState = null,
	lastApplicationState = null,
	currentView = null;

function log(message) {
    var wrap = $('#log');

    wrap.append('<div>' + message + '</div>');

    wrap.prop(
        'scrollTop',
        wrap.prop('scrollHeight')
    );
}

function bootstrap() {
    log('Bootstrapping');

    setupControls();
    setupMainMenu();
    setupPidConfiguration();
    setupConsole();
    updateDevices();

    //$(document.body).addClass('show-log');
}

function setupControls() {
    var actions = {
        startReflow: function() {
            sendIntent(Intent.START_REFLOW);
        },
	    stopReflow: function() {
		    sendIntent(Intent.MAIN_MENU);
	    }
    };

    $('.action-btn').each(function() {
       $(this).click(function() {
           var action = $(this).data('action');

           if (typeof(actions[action]) === 'function') {
               actions[action].call(actions[action]);
           }
       })
    });
}

function setupMainMenu() {
    $('#main-menu > LI').click(function() {
        var viewName = $(this).data('view');

        showView(viewName);
    });
}

function setupPidConfiguration() {
	$('.pid-parameter').change(function() {
		sendPidValues();
	});
}

function setupConsole() {
    $('#user-message').click(function() {
        if ($(this).text() === 'send message') {
            $(this).text('').removeClass('placeholder');
        }
    });

    $('#user-message').blur(function() {
        if ($(this).text() === '') {
            $(this).text('send message').addClass('placeholder');
        }
    });

    $('#send-message-btn').click(function() {
        var message =  $('#user-message').text();

        if (message.length === 0) {
            return;
        }

        native.sendMessage(message);
    });
}

function sendRequest(type, data) {
	native.sendMessage('<' + JSON.stringify({
		type: type,
		data: data || {}
	}) + '>');
}

function sendIntent(intent) {
	sendRequest('intent', {intent: intent});
}

function showView(name) {
	var viewMethodName = 'show' + convertActionName(name) + 'View';

	if (typeof(window[viewMethodName]) === 'function') {
		window[viewMethodName]();
	}

    $('.view').hide();
    $('.view[data-name=' + name + ']').show();

    $('#main-menu > LI').removeClass('active');
    $('#main-menu > LI[data-view=' + name + ']').addClass('active');

	currentView = name;
}

function updateView() {
	if (currentView === null) {
		return;
	}

	showView(currentView);
}

function showPidView() {
	sendRequest('get-pid');

	if (currentApplicationState === 'main-menu') {
		sendIntent(Intent.CONFIGURE_PID);
	}
}

function showReflowView() {
	if (currentApplicationState === 'reflow') {
		$('#start-reflow-btn').hide();
		$('#stop-reflow-btn').show();
	} else {
		$('#start-reflow-btn').show();
		$('#stop-reflow-btn').hide();
	}
}

function sendPidValues() {
	var p = parseFloat($('#pid-p').val()),
		i = parseFloat($('#pid-i').val()),
		d = parseFloat($('#pid-d').val());

	sendRequest('set-pid', {
		p: p,
		i: i,
		d: d
	});
}

function updateDevices() {
    var wrap = $('#paired-devices-list'),
        i;

    wrap.empty();

    var devices = JSON.parse(native.getPairedDevices());

    log('Paired: ' + JSON.stringify(devices));

    for (i = 0; i < devices.length; i++) {
        wrap.append('<li class="device-item' + (devices[i] === connectedDeviceName ? ' connected' : '') + '" data-name="' + devices[i] + '">' + devices[i] + '' + (devices[i] === connectedDeviceName ? ' (connected)' : '') + '</li>');
    }

    $('.device-item').click(function() {
        var name = $(this).data('name');

        native.connectDevice(name);
    });
}

function handleRequest(request) {
	switch (request.type) {
		case 'pid':
			handlePidRequest(request.data);
		break;

		case 'state-changed':
			handleStateChangedRequest(request.data);
		break;
	}
}

function handlePidRequest(data) {
	$('#pid-p').val(parseFloat(data.p));
	$('#pid-i').val(parseFloat(data.i));
	$('#pid-d').val(parseFloat(data.d));
}

function handleStateChangedRequest(data) {
	lastApplicationState = currentApplicationState;
	currentApplicationState = data.name;

	if (lastApplicationState !== null) {
		$(document.body).removeClass('state-' + lastApplicationState.toLowerCase());
	}

	$(document.body).addClass('state-' + currentApplicationState.toLowerCase());

	updateView();
}

function showConsole(deviceName) {
    showView('console');
}

function appendConsole(type, message) {
    var wrap = $('#message-list');

    wrap.append('<li class="' + type + '">' + message.replace('<', '&lt;').replace('>', '&gt;') + '</li>');

    wrap.prop(
        'scrollTop',
        wrap.prop('scrollHeight')
    );
}

function convertActionName(name) {
	var dashPos;

	while ((dashPos = name.indexOf('-')) != -1) {
		name = name.substr(0, dashPos) + (name.substr(dashPos + 1, 1)).toUpperCase() + name.substr(dashPos + 2);
	}

	return name.substr(0, 1).toUpperCase() + name.substr(1);
}

function onBluetoothReady() {
    log('Bluetooth ready');

    updateDevices();
    showView('device-choice');
}

function onBluetoothConnected(deviceName) {
    log('Bluetooth connected: ' + deviceName);

	connectedDeviceName = deviceName;

	updateDevices();
	showView('device-choice');
}

function onBluetoothStateChanged(newState) {
    log('Bluetooth state changed to: ' + newState);

	lastBluetoothState = currentBluetoothState;
	currentBluetoothState = newState;

	if (currentBluetoothState !== 'connected') {
		connectedDeviceName = null;
	}

	$('#state').html(newState);

	if (lastBluetoothState !== null) {
		$(document.body).removeClass('state-' + lastBluetoothState.toLowerCase());
	}

	$(document.body).addClass('state-' + currentBluetoothState.toLowerCase());

	if (currentBluetoothState.toLowerCase() === 'connected') {
		sendRequest('get-state');
	}

	updateDevices();
}

function onBluetoothMessageReceived(message) {
    log('Bluetooth message received: ' + message);

    appendConsole('rx', '< ' + message);

	if (message.substr(0, 1) === '{') {
		var parsedMessage;

		try {
			parsedMessage = JSON.parse(message);
		} catch (e) {
			appendConsole('error', '@ Parsing message "' + message + '" failed');
		}

		try {
			handleRequest(parsedMessage);
		} catch (e) {
			appendConsole('error', '@ Handling request for "' + JSON.stringify(parsedMessage) + '" failed');
		}
	}
}

function onBluetoothMessageSent(message) {
    log('Bluetooth message sent: ' + message);

    appendConsole('tx', '> ' + message);
}

function onBluetoothConnectionFailed() {
    log('Bluetooth connection failed');

    updateDevices();
    showView('device-choice');
}

function onBluetoothConnectionLost() {
    log('Bluetooth connection lost');

    updateDevices();
    showView('device-choice');
}

$(document).ready(function() {
    bootstrap();
});