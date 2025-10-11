var HOST = "ws://localhost:3000";
var ws = new WebSocket(HOST);

ws.onopen = () =>{
    console.log("Hey Do You See Me");
}

ws.onmessage = (event) => {
    console.log("Raw Message: ", event.data);
}
