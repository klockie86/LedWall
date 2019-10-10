const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Omexom Led Wall</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="mobile-web-app-capable" content="yes">
</head>
<body>

<div id="command">
<h1>Omexom ping pong led wall</h1>
  <button type="button" onclick="setState(0)">uit</button>
  <button type="button" onclick="setState(1)">rood</button>
	<button type="button" onclick="setState(2)">groen</button>
  <button type="button" onclick="setState(3)">blauw</button>
  <button type="button" onclick="setState(4)">flipmode</button><BR>
</div>

Led status:<span id="Status">0</span></br>
<div class="slidecontainer">
  <input type="range" min="0" max="255" value="50" class="slider" id="setBrightness"/>
</div>
  <input type="color" id="colorpicker" value="#0071ce">
  <button type="submit" name="state" onclick="setColor()" >toepassen</button>
</div>
<div id="climate"></div>
</br>
</br>
</br>
</br>
</br>
</br>
</br>
</br>
</br>
</br>
<input type="button" id = "wifi_button" onclick="resetWifi()" value="Reset Wifi"/>
</br>
<span id="resetStatus"></span></br>
<div id="footer"><p>Designed by Jeroen Klock</p></div>
<script>
function setState(led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Status").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setState?LEDstate="+led, true);
  xhttp.send();
}

function hexToRgb(hex) {
  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16),
    g: parseInt(result[2], 16),
    b: parseInt(result[3], 16)
  } : null;
}

function setColor() {
  var color = document.getElementById("colorpicker").value;
  var colorRGB = hexToRgb(color);
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "setColor?r="+colorRGB.r+"&g="+colorRGB.g+"&b="+colorRGB.b, true);
  xhttp.send();
}

setBrightness.onchange = function() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "setBrightness?Brightness="+this.value, true);
  xhttp.send();
}


function resetWifi() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("resetStatus").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "resetWifi", true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 5000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("climate").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readTemp", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";
