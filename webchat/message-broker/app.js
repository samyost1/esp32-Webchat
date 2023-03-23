const input = document.querySelector('input');
const button = document.querySelector('button');
const messages = document.querySelector('#messages');
const userId_heading = document.querySelector('#user-id_heading');


const clientId = 'student-' + Math.random().toString(16).substr(2, 4)
userId_heading.textContent = 'User-id: ' + clientId


const host = 'wss://netwerkenbasis.com:1884'

const options = {
	username: 'student',
    password: 'welkom01',
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
})


// Received
connection.on('message', (topic, message, packet) => {
	var message_clientId = message.toString().substring(0, 12)
	if (message_clientId != clientId) {
		const el = document.createElement('li');
		el.innerHTML = message;
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
		connection.publish('chat/message', input.value)
		input.value = '';
	}
}