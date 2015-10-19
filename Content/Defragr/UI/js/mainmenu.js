function _MainMenu()
{

}

_MainMenu.prototype = {

	/*======================================================*\
	|* PUBLIC
	\*======================================================*/


	/*======================================================*\
	|* INTERNAL
	\*======================================================*/

	_init: function()
	{
		UE4.getFileList(UE4.getMapsDir(), function(files)
		{
			for(i = 0; i < files.length; i++)
			{
				$('.map-list ul').append("<li>" + files[i] + "</li>");
			}
		});
	}
}
var MainMenu = new _MainMenu();


// Initialize the virtual console
Zepto(function($){

	mobileConsole.options({
		showOnError: true,
		proxyConsole: true,
		isCollapsed: true,
		catchErrors: true
	});
});

// Initialize the main menu application
UE4.ready(function(){ MainMenu._init(); });