const Express = require('express');
const SocketIO = require('socket.io');
const SerialPort = require('serialport');
const Http = require('http');

const app = Express();
const server = Http.createServer(app);
const io = SocketIO.listen(server);

io.on('connect', socket =>{
	console.log("Se conectó un nuevo socket");
})

app.use(Express.static(__dirname + "/public/"));

app.get('/',(req, res, next)=>{
	res.sendFile(__dirname + "/index.html");
});

const Readline = SerialPort.parsers.Readline;
const parser = new Readline('\n');

const mySerial = new SerialPort("/dev/cu.usbmodem1421",{
	baudRate: 9600
});

mySerial.pipe(parser);

mySerial.on('open', ()=>{
	console.log("Puerto Serial Abierto");
	parser.on('data', data =>{
		console.log(data.toString());
		io.emit('arduino:data',{
			value: data.toString()
		});
	});
	mySerial.on('error', err =>{
		console.log(err.message);
	});
});

server.listen(3000,()=>{
	console.log("Escuchando en puerto ", 3000);
});