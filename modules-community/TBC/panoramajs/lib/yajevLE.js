/*************************************************
 *      Version light de Yajev : Yaj3v :         *
 * Yet Another JavaScript Virtual Visit Viewer   *
 *                                               *
 * Documentation et aide disponible sur mon site *
 *	- Site Web : http://astre.henri.free.fr      *
 *	- Développé par Henri ASTRE                  *
 *	                                             *
 *************************************************/

/***************************
 *	Définition de YajevLE  *
 ***************************/

function YajevLE(identifiant)
{
	// variables de YajevLE
	this.pic         = "url(logo.jpg)"; // nom du fichier image sous la forme url(pano/pano0.jpg)
	this.width       = 640;                   // largeur du panorama (pas du fichier jpeg)
	this.height      = 480;                   // hauteur du panorama (pas du fichier jpeg)
	this.identifiant = identifiant;           // identifiant du div contenant le panorma
	this.dec         = 0;                     // decalage initial du panorama
	this.stepSize    = 20;                    // pas de decalage en pixel
	this.quickness   = 40;                    // intervalle de temps entre chaque decalage en ms
	this.panoWidth   = 5200;                  // largeur du panorama en pixel (recalculé automatiquement)
	this.panoHeight  = 480;                   // hauteur du panorama en pixel (recalculé automatiquement)
	this.actif       = 0;
	this.angle       = 0;                     // angle correspondant à la rotation du panorama
	this.autoRotate  = false;
	this.div         = FindID(identifiant);   // div qui contient le panorama
	this.mouseX      = 0;
	this.maxSpeed    = 2;
	this.loaded      = false;
	this.tabLoad     = new Array();
	
	//méthodes de YajevLE
	this.getCssProperty   = YajevLE_getCssProperty; // permet d'initialiser YajevLE avec les propriétés CSS
	this.refresh          = YajevLE_refresh;        // permet de déplacer le panorama et de
	this.startMoveLeft    = YajevLE_startMoveLeft;  // permet de lancer moveLeft  à intervalle régulier
	this.startMoveRight   = YajevLE_startMoveRight; // permet de lancer moveRight à intervalle régulier
	this.moveLeft         = YajevLE_moveLeft;       // permet de déplacer le panorama vers la gauche
	this.moveRight        = YajevLE_moveRight;      // permet de déplacer le panorama vers la droite
	this.stopMove         = YajevLE_stopMove;       // permet de stopper le mouvement du panorama
	this.getAngle         = YajevLE_getAngle;       // permet de récupérer l'angle correspondant à la rotation du panorama
	this.setAngle         = YajevLE_setAngle;       // permet de faire tourner le panorama d'un certain angle
	this.setQuickness     = YajevLE_setQuickness;   // permet de spécifier l'intervalle de temps en ms entre chaque déplacement du panorama (réglage de la fluidité)
	this.setStepSize      = YajevLE_setStepSize;    // permet de spécifier de combien de pixel se déplace le panorama à chaque déplacement
	this.getPanoWidth     = YajevLE_getPanoWidth;   // donne la largeur du fichier image du panorama
	this.getPanoHeight    = YajevLE_getPanoHeight;  // donne la hauteur du fichier image du panorama
	this.getWidth         = YajevLE_getWidth;       // donne la largeur du div de la zone panoramique
	this.getHeight        = YajevLE_getHeight;      // donne la hauteur du div de la zone panoramique
	this.getShift         = YajevLE_getShift;
	this.setShift         = YajevLE_setShift;
	this.getStepSize      = YajevLE_getStepSize;
	this.getQuickness     = YajevLE_getQuickness;
	this.setAutoRotate    = YajevLE_setAutoRotate;
	this.setMaxSpeed      = YajevLE_setMaxSpeed;
	this.getMousePos      = YajevLE_getMousePos;
	this.addElementToLoad = YajevLE_addElementToLoad;
	this.buildHtml        = YajevLE_buildHtml;
	this.launch           = YajevLE_launch;
	this.tryLaunch        = YajevLE_tryLaunch;
	this.destroy          = YajevLE_destroy;
	this.getCssProperty();
}

/***************************
 *	méthodes de YajevLE    *
 ***************************/

function YajevLE_addElementToLoad(type, variable) {
	this.tabLoad.push({t: type, v: variable});
}

function YajevLE_buildHtml() {
	this.scrollWidth   = this.width / 3;
	this.div.innerHTML = '<div id="' + this.identifiant + 'scrollLeft" onmousemove=\"' + this.identifiant + '.getMousePos(event);\" onmouseover=\"' + this.identifiant + '.startMoveLeft();\"  onmouseout="' + this.identifiant + '.stopMove();" class=\"myPanoscrollLeft\" style=\"float: left;  height: ' + this.height + 'px; width: ' + this.scrollWidth + 'px; margin-bottom: 0px;\"></div>';
									 
	this.div.innerHTML += '<div id="' + this.identifiant + 'scrollRight" onmousemove=\"' + this.identifiant + '.getMousePos(event);\" onmouseover=\"' + this.identifiant + '.startMoveRight();\" onmouseout="' + this.identifiant + '.stopMove();" class=\"myPanoscrollRight\" style=\"float: right; height: ' + this.height + 'px; width: ' + this.scrollWidth + 'px; margin-bottom: 0px;\"></div>';
	
	var scrollLeft = FindID(this.identifiant + 'scrollLeft');
	//this.scrollLeftOffset  = scrollLeft.offsetLeft;
	this.scrollLeftOffset = 0;

	while (scrollLeft.offsetParent) {
		this.scrollLeftOffset += scrollLeft.offsetLeft;
		scrollLeft             = scrollLeft.offsetParent;
	}

	var scrollRight = FindID(this.identifiant + 'scrollRight');
	//this.scrollRightOffset  = scrollRight.offsetLeft;
	this.scrollRightOffset = 0;

	while (scrollRight.offsetParent) {
		this.scrollRightOffset += scrollRight.offsetLeft;
		scrollRight             = scrollRight.offsetParent;
	}
}

function YajevLE_launch()
{
	var local_this = this;
	if (arguments.length > 0) {
		local_this.actif = setInterval(
			function () {
				local_this.tryLaunch(1);},
			local_this.quickness
		);
	} else {
		local_this.actif = setInterval(
			function () {
				local_this.tryLaunch();},
			local_this.quickness
		);
	}
}

function YajevLE_tryLaunch()
{
	for (var i = 0; i < this.tabLoad.length; ++i) {
		switch (this.tabLoad[i].t) {
			default :
			case 'pic': if (this.tabLoad[i].v.complete == false) {
					return false;
			}
						 break;
			case 'var': if (this.tabLoad[i].v == false) {
					return false;
			}
						 break;
		}
	}
	this.panoWidth  = this.pano.width;
	this.panoHeight = this.pano.height;
	if (arguments.length == 0) {
		this.buildHtml();
	}
	this.loaded = true;
	clearInterval(this.actif);
	this.refresh();
}

function YajevLE_getCssProperty() {
	this.width  = parseInt(this.div.style.width);
	this.height = parseInt(this.div.style.height);
	
	if (this.div.style.backgroundPosition) {
		this.dec = parseInt(this.div.style.backgroundPosition);
	} else {
		this.dec = 0;
	}
		
	var panoUrl   = extractUrl(this.div.style.backgroundImage);
	this.pano     = new Image;
	this.pano.src = panoUrl;
	this.addElementToLoad('pic',this.pano);
}

function YajevLE_refresh() {
	if (this.loaded) {
		this.angle = (360 - (this.dec * 360 / this.panoWidth)) % 360;
	}
	this.div.style.backgroundPosition = this.dec + 'px 0px';
	if (this.onstatechange) {
		this.onstatechange();
	}
}

function YajevLE_startMoveLeft() {
	if (this.autorotate) {
		return;
	}
		
	if (arguments.length > 0) {
		var speed        = arguments[0];
		var local_this   = this;
		local_this.actif = setInterval(
			function () {
				local_this.moveLeft(speed);},
			local_this.quickness
		);
	} else {
		var local_this   = this;
		local_this.actif = setInterval(
			function () {
				local_this.moveLeft();},
			local_this.quickness
		);
	}
}

function YajevLE_startMoveRight() {
	if (this.autorotate) {
		return;
	}

	if (arguments.length > 0) {
		var speed        = arguments[0];
		var local_this   = this;
		local_this.actif = setInterval(
			function () {
				local_this.moveRight(speed);},
			local_this.quickness
		);
	} else {
		var local_this   = this;
		local_this.actif = setInterval(
			function () {
				local_this.moveRight();},
			local_this.quickness
		);
	}
}

function YajevLE_moveLeft() {
	if (arguments.length > 0) {
		var speed = arguments[0];
	} else {
		var relativeX = this.mouseX - this.scrollLeftOffset;
		var speed     = (1 - relativeX / this.scrollWidth) * this.maxSpeed;
	}
	this.dec = parseInt((this.dec + this.stepSize * speed + this.panoWidth) % this.panoWidth);
	this.refresh();
}

function YajevLE_moveRight() {
	if (arguments.length > 0) {
		var speed = arguments[0];
	} else {
		var relativeX = this.mouseX - this.scrollRightOffset;
		var speed     = (relativeX / this.scrollWidth) * this.maxSpeed;
	}
	this.dec = parseInt((this.dec - this.stepSize * speed + this.panoWidth) % this.panoWidth);
	this.refresh();
}

function YajevLE_stopMove() {
	this.autorotate = false;
	clearInterval(this.actif);
}

function YajevLE_getAngle() {
	return this.angle;
}

function YajevLE_setAngle(angle) {
	this.angle = angle;
	this.dec   = Math.round(this.panoWidth * angle / 360);
	this.refresh();
}

function YajevLE_setQuickness(quickness) {
	this.quickness = quickness;
}

function YajevLE_setStepSize(stepSize) {
	this.stepSize = stepSize;
}

function YajevLE_getPanoWidth() {
	return this.panoWidth;
}

function YajevLE_getPanoHeight() {
	return this.panoHeight;
}

function YajevLE_getWidth() {
	return this.width;
}

function YajevLE_getHeight() {
	return this.height;
}

function YajevLE_getShift() {
	return this.dec;
}
function YajevLE_setShift(shift) {
	this.dec = shift;
	this.refresh();
}

function YajevLE_getQuickness(quickness) {
	return this.quickness;
}

function YajevLE_getStepSize(stepSize) {
	return this.stepSize;
}

function YajevLE_setAutoRotate(speed) {
	this.startMoveRight(speed,1);
}

function YajevLE_setMaxSpeed(speed) {
	this.maxSpeed = speed;
}

function YajevLE_getMousePos(e) {
	if (!e) { //IE
		var e = window.event
	}
	if (e.pageX) { //Moz
		this.mouseX = e.pageX;
	} else if (e.clientX) { //IE
		this.mouseX = e.clientX - 2;
	}
}

function YajevLE_destroy() {
	this.div.innerHTML = "";
}

/******************************************
 *	Ajout de fonctionnalités à YajevLE    *
 ******************************************/

YajevLE.prototype.loadPano = YajevLE_loadPano;     // permet de changer l'image du panorama

function YajevLE_loadPano(panoUrl)
{
	this.loaded                    = false;
	this.div.style.backgroundImage = 'url(' + panoUrl + ')';
	this.pano                      = new Image;
	this.pano.src                  = panoUrl;
	this.addElementToLoad('pic',this.pano);
	this.launch(1);
}

//permet de transformer "url(pano/pano0.jpg)" en "pano/pano0.jpg"
function extractUrl(strIn) {
	var strOut = strIn.substring(4,strIn.length - 1);

	if (strOut.charAt(0) == '"') { //Merci Opera... d'utiliser : url("pano/pano0.jpg") ;-(
		strOut = strOut.substring(1,strOut.length - 1);
	}
	
	return strOut;
}
