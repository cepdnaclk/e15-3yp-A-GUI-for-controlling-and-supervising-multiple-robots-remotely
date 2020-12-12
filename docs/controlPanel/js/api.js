function connect(ip) {
   robotIp = $("#selectIP").val();

   $.ajax({
      type: "GET",
      dataType: "json",
      url: "http://" + robotIp + "/status",
      success: function (data) {

         var response = data;

         robotVersion = response.version;
         robotStatus = response.status;

         if(robotStatus == "Active"){
            $("#txtStatus").val(robotStatus);
            $("#txtVersion").val(robotVersion)

            log("Connection:" + robotIp + " "+ robotStatus + "(" + robotVersion+ ")");
            return true;

         }else{
            return false;
         }

      },
      error: function (request, status, err) {
         console.error(request + " " + status);


         alert("Connection refused !");

         $("#txtStatus").val("Not Connected");
         $("#txtVersion").val("N/A");
         log("Connection: Failed");

         return false;
      },
      timeout: 1000
   });

   return false;
}

function send(cmd) {

   var ip = "http://" + $("#selectIP").val();

   if (cmd == 8) {
      url = ip + "/motor?dir=forward";

   } else if (cmd == 2) {
      url = ip + "/motor?dir=backward";

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
         $("#distFiltered").html(response.filtered);
         //log("Color: R:" + response.R + ", G:" + response.G + ", B:" + response.B);

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

function networkSearch(){

   from = 0;
   to = 255;
   d = 0;      // No of active devices

   $("#selectIP").html("<option value=\"192.168.8.102\" disabled selected>192.168.8.102</option>"); // clear the select box

   for(i=from;i<=to;i++){
      robotIp = "192.168.8." + i;

      $.ajax({
         type: "GET",
         dataType: "json",
         url: "http://" + robotIp + "/status",
         success: function (data) {
            var response = data;
            robotVersion = response.version;
            robotStatus = response.status;
            $("#selectIP").append("<option value='"+ response.IP +"'>"+  response.IP + "</option>").val(response.IP);
            d = d + 1;
         },
         error: function (request, status, err) {
            // Nothing to do here
         },
         timeout: 1500
      });
   }
}
