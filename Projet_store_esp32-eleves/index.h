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
      //
      // A COMPLETER
      //
      
    }
  };
  xhttp.open("GET", "GetValues", true);
  xhttp.send();
}

// Demande d'ouverture
function BtnOuvrir() {
  //
  // A COMPLETER
  //
  
}

// Demande de fermeture
function BtnFermer() {
  //
  // A COMPLETER
  //
  
}

// Demande d'arrêt
function BtnArret() {
  //
  // A COMPLETER
  //
  
}

</script>
</body>
</html>
)=====";
