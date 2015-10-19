window.onload = function(){

	mobileConsole.options({
		showOnError: true,
		proxyConsole: true,
		isCollapsed: true,
		catchErrors: true
	});

	GetFileList("E:\\Unreal Projects\\Defragr\\Maps\\", function(data)
	{
	});

};