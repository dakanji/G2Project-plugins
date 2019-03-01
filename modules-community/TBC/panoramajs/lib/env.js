/***********************************************
 *	Module Javascript de MyFlashPano :         *
 *	- Développé par Henri ASTRE                *
 *	- Site Web : http://astre.henri.free.fr    *
 *	                                           *
 *	Gestion des différents environnements      *
 ***********************************************/

/* Fonction de recherche d'un élément ID compatible avec tous les navigateurs */
function FindID(id)
{
	if (document.layers) {
		return document.layersid;
	}

	if (document.all && !document.getElementById) {
		return document.allid;
	}

	if (document.all && document.getElementById) {
		return document.getElementById(id);
	}
	
	if (!document.all && document.getElementById) {
		return document.getElementById(id);
	}
}
