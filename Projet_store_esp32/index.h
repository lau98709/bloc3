const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<meta charset="utf-8">
<style>
.card{
    max-width: 400px;
    background: #02b875;
    color: #FFFFFF;
    font-size: 18px;
}
body{
    font-size: 16px;
}
</style>
<body>

<div class="card">
  <h1>Etat du store :<span id="etat">0</span></h1><br>
</div>
<div class="capteur">
  Fenetre ouverte : <span id="fo">0</span><br>
  Fenetre fermee : <span id="ff">0</span><br>
  Bouton ouverture : <span id="bo">0</span><br>
  Bouton fermeture : <span id="bf">0</span><br>
</div>
<div class="bouton">
  <button id="Arret" onclick="BtnArret()">Arrêter</button>
  <button id="Ouvrir" onclick="BtnOuvrir()">Ouvrir</button>
  <button id="Fermer" onclick="BtnFermer()">Fermer</button>
</div>

<script>


setInterval(function() {
  // Mise à jour des donnees à intervalle regulier
  getData();
}, 500); // update rate in ms

// Ajax pour recuperer les donnees du store
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Interpretation des donnees recues
      var s = this.responseText.split("\n");
      var s2 = s[0].split(":");
      switch (parseInt(s2[1])) {
        case 1: document.getElementById("etat").innerHTML = "Ouverture en cours"; break;
        case 2: document.getElementById("etat").innerHTML = "Fermeture en cours"; break;
        case 0: document.getElementById("etat").innerHTML = "Arret"; break;
      }
      document.getElementById("fo").innerHTML = s[1].split(":")[1];
      document.getElementById("ff").innerHTML = s[2].split(":")[1];
      document.getElementById("bo").innerHTML = s[3].split(":")[1];
      document.getElementById("bf").innerHTML = s[4].split(":")[1];
    }
  };
  xhttp.open("GET", "GetValues", true);
  xhttp.send();
}

// Demande d'ouverture
function BtnOuvrir() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var s = this.responseText.split("\n");
    }
  };
  xhttp.open("GET", "BtnOuvrir", true);
  xhttp.send();
}

// Demande de fermeture
function BtnFermer() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var s = this.responseText.split("\n");
    }
  };
  xhttp.open("GET", "BtnFermer", true);
  xhttp.send();
}

// Demande d'arrêt
function BtnArret() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var s = this.responseText.split("\n");
    }
  };
  xhttp.open("GET", "BtnArret", true);
  xhttp.send();
}

</script>
</body>
</html>
)=====";
