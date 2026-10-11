#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* SSID = "Free_WiFi";
const char* PASS = "";

const char* PRANK_HTML = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Loading...</title>
<style>
  html,body{margin:0;padding:0;height:100%;background:#000;overflow:hidden;color:#fff;font-family:Arial;}
  #start{position:fixed;inset:0;display:flex;flex-direction:column;align-items:center;justify-content:center;background:#000;cursor:pointer;z-index:99999;}
  #start h1{font-size:22px;margin-bottom:30px;}
  #btn{background:#00cc44;color:#fff;border:none;padding:18px 40px;font-size:20px;border-radius:8px;box-shadow:0 0 20px rgba(0,204,68,.6);}
  .hidden{display:none!important;}
  .virus{position:fixed;background:#000;border:2px solid #fff;color:#fff;font-family:monospace;text-align:center;padding:8px;z-index:9999;box-shadow:0 0 15px #fff;animation:blink .25s infinite,shake .15s infinite;pointer-events:none;font-weight:bold;}
  .virus h3{margin:0;font-size:16px;}
  @keyframes blink{0%,100%{color:#fff;border-color:#fff;background:#000;}50%{color:#000;border-color:#000;background:#fff;}}
  @keyframes shake{0%{transform:translate(0,0);}25%{transform:translate(2px,2px);}50%{transform:translate(-2px,-2px);}75%{transform:translate(2px,-2px);}100%{transform:translate(0,0);}}
  #flash{position:fixed;inset:0;background:#fff;z-index:99998;display:none;animation:flash .1s infinite;}
  @keyframes flash{0%,100%{opacity:0;}50%{opacity:.8;}}
</style>
</head>
<body>
<div id="start">
  <h1>Загрузка контента...</h1>
  <button id="btn">▶ Нажми, чтобы показать</button>
</div>
<div id="flash"></div>
<script>
var started=false;
function spawn(){var w=document.createElement('div');w.className='virus';w.style.left=Math.random()*(innerWidth-150)+'px';w.style.top=Math.random()*(innerHeight-80)+'px';w.style.width='150px';w.innerHTML='<h3>YOU ARE AN IDIOT</h3>';document.body.appendChild(w);}
document.getElementById('btn').onclick=function(){
  if(started)return;started=true;
  document.getElementById('start').classList.add('hidden');
  document.getElementById('flash').style.display='block';
  setInterval(spawn,200);spawn();
};
</script>
</body>
</html>
)rawliteral";

WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;

void handleRoot() {
  server.send(200, "text/html", PRANK_HTML);
}

void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASS);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
