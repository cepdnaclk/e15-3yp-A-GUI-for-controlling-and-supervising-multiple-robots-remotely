

function readCurrentValues(){
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/eeprom/read",

    success: function (data) {
      var response = data;
      leftSC = response.lftSC;
      rightSC = response.rgtSC;
      settingRbtId = response.rbtId;

      if(robotStatus == "Active"){
        $("#leftSpeedCorrect").val(leftSC);
        $("#rightSpeedCorrect").val(rightSC);
        $("#settingRobotId").val(settingRbtId);

        return true;

      }else{
        return false;
      }

    },
    error: function (request, status, err) {
      console.error(request + " " + status);

      $("#leftSpeedCorrect").val(-1);
      $("#rightSpeedCorrect").val(-1);
      $("#settingRobotId").val(-1);


      return false;
    },
    timeout: 1000
  });

  return false;
}


function eepromUnlock(){
  updateEEPROM(0,1);
}

function updateLeftSpeedCorrect() {
  var value =  $("#leftSpeedCorrect").val();
  if (-50<=value && value<=50){
    updateEEPROM(4,value);
  }else{
    alert("Enter a value between -50 and 50");
  }
}

function updateRightSpeedCorrect() {
  var value =  $("#rightSpeedCorrect").val();
  if (-50<=value && value<=50){
    updateEEPROM(3,value);
  }else{
    alert("Enter a value between -50 and 50");
  }
}

function updateSettingRobotId() {
  var value = $("#settingRobotId").val();
  if (0<=value && value<=15){
    updateEEPROM(1,value);
  }else{
    alert("Enter a value between 0 and 15");
  }
}

function updateEEPROM(id,value){
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/eeprom/write?id="+id+"&value="+value,

    success: function (data) {
      var response = data;
      console.log(response);
      return true;
    },
    error: function (request, status, err) {
      console.error(request + " " + status);
      return false;
    },

    timeout: 1000
  });

  return false;
}
