function updateAccel() {
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/accel",

    success: function (data) {

      var response = data;
      console.log(response);

      accelX = Math.floor(response.x * 100) / 100;
      accelY = Math.floor(response.y * 100) / 100;
      accelZ = Math.floor(response.z * 100) / 100;

      $("#accelX").html(Math.floor(response.x * 100) / 100);
      $("#accelY").html(Math.floor(response.y * 100) / 100);
      $("#accelZ").html(Math.floor(response.z * 100) / 100);

      log("Accelerometer: x:" + accelX + ", y:" + accelY + ", z:" + accelZ);
      return true;
    },
    error: function (request, status, err) {
      console.error(request + " " + status);

      accelX = Math.floor(response.x * 100) / 100;
      accelX = Math.floor(response.y * 100) / 100;
      accelX = Math.floor(response.z * 100) / 100;

      log("Accelerometer: Error");
      $("#accelX").val("0.00");
      $("#accelY").val("0.00");
      $("#accelZ").val("0.00");

      return false;
    },
    timeout: 1000
  });

  return false;
}

function updateComp() {
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/mag",

    success: function (data) {

      var response = data;
      console.log(response);

      compX = Math.floor(response.x * 100) / 100;
      compY = Math.floor(response.y * 100) / 100;
      compZ = Math.floor(response.z * 100) / 100;
      compHeading = Math.floor(response.heading * 100) / 100

      $("#compX").html(compX);
      $("#compY").html(compY);
      $("#compZ").html(compZ);
      $("#compHeading").html(compHeading);

      log("Compass: x:" + compX + ", y:" + compY + ", z:" + compZ + ", head:" + compHeading);
      return true;
    },
    error: function (request, status, err) {
      console.error(request + " " + status);

      alert("Connection refused !");
      $("#compX").val("0.00");
      $("#compY").val("0.00");
      $("#compZ").val("0.00");
      log("Compass: Error");
      return false;
    },
    timeout: 1000
  });

  return false;
}


function updateProximity(){
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/dist",

    success: function (data) {
      var response = data;
      console.log(response);

      $("#distRaw").html(response.raw);
      $("#distFiltered").html(Math.round(response.filtered));
      log("Proximity: raw>" + response.raw + " filtered>" + response.filtered );

      return true;
    },
    error: function (request, status, err) {
      console.error(request + " " + status);
      log("Proximity: Error");
      return false;
    },
    timeout: 1000
  });

  return false;
}


function rgb2hex(rgb) {
  var hex = Number(rgb).toString(16);
  if (hex.length < 2) {
    hex = "0" + hex;
  }
  return hex;
}

function updateColor() {
  robotIp = $("#selectIP").val();

  $.ajax({
    type: "GET",
    dataType: "json",
    url: "http://" + robotIp + "/color",

    success: function (data) {

      var response = data;
      console.log(response);

      $("#colorRed").html(response.R);
      $("#colorGreen").html(response.G);
      $("#colorBlue").html(response.B);
      $("#colorShow").css('background', "#" + rgb2hex(response.R) + rgb2hex(response.G)+ rgb2hex(response.B) );
      console.log("#" + rgb2hex(response.R) + rgb2hex(response.G)+ rgb2hex(response.B));
      temp = response.temp;

      log("Color: R:" + response.R + ", G:" + response.G + ", B:" + response.B);

      return true;
    },
    error: function (request, status, err) {
      console.error(request + " " + status);
      log("Color: Error");
      return false;
    },
    timeout: 2000
  });

  return false;
}
