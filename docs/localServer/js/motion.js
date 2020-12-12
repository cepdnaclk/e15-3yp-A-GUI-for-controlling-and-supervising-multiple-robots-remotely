function send(cmd) {

  var ip = "http://" + $("#selectIP").val();

  if (cmd == 8) {
    url = ip + "/motor?dir=forward&spd=" + $("#speed").val();

  } else if (cmd == 2) {
    url = ip + "/motor?dir=backward&spd=" + $("#speed").val();

  } else if (cmd == 6) {
    //url = ip + "/motor?dir=rotCW";
    url = ip + "/turn?ang="+ $("#rotAngle").val();
  } else if (cmd == 4) {
    //url = ip + "/motor?dir=rotCCW";
    url = ip + "/turn?ang=-" + $("#rotAngle").val();

  } else {
    url = ip + "/motor?dir=stop"
  }

  $.ajax({
    type: "GET",
    dataType: "json",
    url: url,
    success: function (data) {
      var response = data;

      //robotVersion = response.version;
      robotStatus = response.status;

      if(robotStatus == "success"){
        log("Motion: " + cmd + ": " + robotStatus);
        return true;
      }else{
        return false;
      }
    },
    error: function (request, status, err) {
      console.error(request + " " + status);


      //alert("Connection refused !");

      $("#txtStatus").val("Not Connected");
      $("#txtVersion").val("N/A");

      return false;
    },
    timeout: 1000
  });

}

$("#txtAngle").html($("#rotAngle").val());

$("#rotAngle").change(function(){
   $("#txtAngle").html(this.value);
});

$("#txtSpeed").html($("#speed").val());

$("#speed").change(function(){
   $("#txtSpeed").html(this.value);
});
