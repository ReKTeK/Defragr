/**
 * Majik UE4 class. Handles all communications with UE4
 * and holds all UE4 related information and methods.
 *
 * NOTE: This is meant to be a static class (Declaration is mainly for
 * aestetic purposes). Use UE4 object instance to access the methods.
 */
function _UE4()
{
	/*======================================================*\
	|* PUBLIC
	\*======================================================*/

	this.rootDirectory = "";
	

	/*======================================================*\
	|* INTERNAL
	\*======================================================*/

	this._callbackQueue = [];
	this._readyQueue = [];


	// Initialize code that relies on this class
	var self = this;
	Zepto(function($){ self._execReady(); });
}

_UE4.prototype = {

	/*======================================================*\
	|* PUBLIC
	\*======================================================*/

	getFileList: function(path, callback)
	{
		data = {'path': path};
		this.execute('getFileList', data, function(data){
			callback(data.files);
		});
	},

	getRootDir: function()
	{
		return this.rootDirectory;
	},

	getMapsDir: function()
	{
		return this.rootDirectory + "Maps/";
	},

	/**
	 * Execute a function in UE4.
	 * 
	 * @param  String     funcName - Function to call in UE4.
	 * @param  Object     data     - Mostly JSON. To be passed to the function.
	 * @param  Function   callback - Function called once UE4 is done executing.
	 */
	execute: function(funcName, data, callback)
	{
		data.callbackID = -1;
		
		// Push the callback on to the queue and add the ID
		// to the data that's passed to UE4
		if(typeof callback !== "undefined")
		{
			var id = this._callbackQueue.indexOf(null);
			if(id > -1)
			{
				this._callbackQueue[callbackID] = callback;
			}
			else
			{
				this._callbackQueue.push(callback);
				id = this._callbackQueue.length - 1;
			}
			data.callbackID = id;
		}

		blu_event(funcName, JSON.stringify(data));
	},

	ready: function(callback)
	{
		this._readyQueue.push(callback);
	},


	/*======================================================*\
	|* INTERNAL
	\*======================================================*/

	/**
	 * Called by UE4, see BluAPIMananger.cpp.
	 * 
	 * @param  Number   callbackID - Index of the callback in the _callbackQueue.
	 * @param  Object   data       - Mostly JSON.
	 */
	_callback: function(callbackID, data)
	{
		if(callbackID > -1)
		{
			this._callbackQueue[callbackID](data);
			this._callbackQueue[callbackID] = null;
		}
	},

	_execReady: function()
	{
		var self = this;

		async.parallel([
			// Set root directory
			function(callback)
			{
				data = {};
				self.execute('getRootDir', data, function(data) {
					self.rootDirectory = data.rootDir;
					callback();
				});
			}
		],

		// Done
		function(err, results)
		{
			for(i = 0; i < self._readyQueue.length; i++)
			{
				if(self._readyQueue[i])
					self._readyQueue[i]();
			}
		});
	}
};
var UE4 = new _UE4();