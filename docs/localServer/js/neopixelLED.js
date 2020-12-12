
var r0 = document.querySelector('#r0'),
g0 = document.querySelector('#g0'),
b0 = document.querySelector('#b0'),
r_out0 = document.querySelector('#r_out0'),
g_out0 = document.querySelector('#g_out0'),
b_out0 = document.querySelector('#b_out0'),
hex_out0 = document.querySelector('#hex0');

function setColor(){
   var r_hex = parseInt(r0.value, 10).toString(16),
   g_hex = parseInt(g0.value, 10).toString(16),
   b_hex = parseInt(b0.value, 10).toString(16),
   hex = "#" + pad(r_hex) + pad(g_hex) + pad(b_hex);
   hex_out0.style.backgroundColor = hex;
   hex_out0.value = hex;
}

function pad(n){
   return (n.length<2) ? "0"+n : n;
}

r0.addEventListener('change', function() {
   setColor();
   r_out0.value = r0.value;
}, false);

r0.addEventListener('input', function() {
   setColor();
   r_out0.value = r0.value;
}, false);

g0.addEventListener('change', function() {
   setColor();
   g_out0.value = g0.value;
}, false);

g0.addEventListener('input', function() {
   setColor();
   g_out0.value = g0.value;
}, false);

b0.addEventListener('change', function() {
   setColor();
   b_out0.value = b0.value;
}, false);

b0.addEventListener('input', function() {
   setColor();
   b_out0.value = b0.value;
}, false);


function writeColor(){
   var r, g, b;

   r = $("#r_out0").val();
   g = $("#g_out0").val();
   b = $("#b_out0").val();

   robotIp = $("#selectIP").val();

   $.ajax({
      type: "GET",
      dataType: "json",
      url: "http://" + robotIp + "/pixelLED/all?r="+r+"&g="+g+"&b="+b,

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
