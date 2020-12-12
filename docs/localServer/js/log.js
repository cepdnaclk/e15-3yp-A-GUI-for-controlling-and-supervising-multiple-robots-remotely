
function clearLog() {
   $("#txtLog").html("");
}

function clearReceiver(){
   $("#txtRxBox").val("");
}
function clearTransmitter(){
   $("#txtTxBox").val("");
}


function log(text){
   $("#txtLog").append(text + "<br>");
}
