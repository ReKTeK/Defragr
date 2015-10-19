var BluAPIManager = {

	callbackQueue: [],

	/**
	 * Execute a function in UE4.
	 * 
	 * @param  String   funcName 
	 * @param  JSON     data
	 * @param  Function callback
	 */
	execute: function(funcName, data, callback)
	{
		data.callbackID = -1;
		
		// Push the callback on to the queue and add the ID
		// to the data that's passed to UE4
		if(typeof callback !== "undefined")
		{
			var id = BluAPIManager.callbackQueue.indexOf(null);
			if(id > -1)
			{
				BluAPIManager.callbackQueue[callbackID] = callback;
			}
			else
			{
				BluAPIManager.callbackQueue.push(callback);
				id = BluAPIManager.callbackQueue.length - 1;
			}
			data.callbackID = id;
		}

		blu_event(funcName, JSON.stringify(data));
	},

	/**
	 * Called by UE4, nothing to see here.
	 * 
	 * @param  {[type]}   callbackID [description]
	 * @param  {[type]}   data       [description]
	 * @return {Function}            [description]
	 */
	callback: function(callbackID, data)
	{
		if(callbackID > -1)
		{
			console.log(data);

			BluAPIManager.callbackQueue[callbackID](data);
			BluAPIManager.callbackQueue[callbackID] = null;
		}
	}
};

// Root directory of the game
var RootDirectory = "";

function GetFileList(path, callback)
{
	data = {'path': path};
	BluAPIManager.execute('GetFileList', data, function(data){
		callback(data.files)
	});
}

window.onload = function(){
	BluAPIManager.execute('GetFileList', data, function(data){
		callback(data.files)
	});
});