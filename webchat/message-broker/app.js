const input = document.querySelector('input');
const button = document.querySelector('button');
const messages = document.querySelector('#messages');
const userId_heading = document.querySelector('#user-id_heading');


const clientId = 'user-' + Math.random().toString(16).substr(2, 4)
userId_heading.textContent = 'User-id: ' + clientId


const host = 'wss://localhost:1884'

const options = {
	keepalive: 60,
	clientId: clientId,
	protocolId: 'MQTT',
	protocolVersion: 4,
	clean: true,
	reconnectPeriod: 1000,
	connectTimeout: 30 * 1000,
	will: {
		topic: 'WillMsg',
		payload: 'Connection Closed abnormally..!',
		qos: 0,
		retain: false
	},
}

console.log('Connecting mqtt client')
const connection = mqtt.connect(host, options)

connection.on('error', (err) => {
	console.log('Connection error: ', err)
	connection.end()
})

connection.on('reconnect', () => {
	console.log('Reconnecting...')
})

connection.on('connect', () => {
	console.log('Client connected:' + clientId)
	// Subscribe
	connection.subscribe('chat/message', { qos: 0 })
	// connection.subscribe('testtopic', { qos: 0 })
})


// Received
connection.on('message', (topic, message, packet) => {
	var message_clientId = message.toString().substring(0, 9)
	console.log('Received Message: ' + message.toString().substring(9)
		+ '\nOn topic: ' + topic
		+ '\nFrom user: ' + message_clientId)
	if (message_clientId != clientId) {
		// console.log('this is you')
		const el = document.createElement('li');
		el.innerHTML = message_clientId + ': ' + message.toString().substring(9);
		messages.appendChild(el);
	}
})

button.addEventListener('click', sendMessage);

input.addEventListener('keyup', function (event) {
	if (event.keyCode === 13) {
		event.preventDefault();
		sendMessage();
	}
});

function sendMessage() {
	if (input.value) {
		const el = document.createElement('li');
		el.innerHTML = clientId + ': ' + input.value;
		messages.appendChild(el);
		connection.publish('chat/message', clientId + input.value, { qos: 0, retain: false })
		input.value = '';
	}
}