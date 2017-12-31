
var thingspk_cfg = {
    // set your channel id here
    channel_id: 0,
    // set your channel's read api key here
    api_key: '',
    // load new data every 10 seconds
    interval: 10000
};

// load the data, customize this function as needed...
function thingspk_loadData(idx) {
    // variable for the data point
    var p;
    var url = 'https://api.thingspeak.com/channels/' + thingspk_cfg.channel_id + '/feed/last.json?api_key=' + thingspk_cfg.api_key;

    console.log('thingspk_loadData() - idx = ' + idx);

    // get the data from thingspeak
    $.getJSON(url, function(thing_data) {
        // get the data point
        p = thing_data[gauge_cfg[idx].data_channel];

        if(p) {
            if(gauge_cfg[idx].round) displayData(idx, Math.round(p));
            else displayData(idx, p);
        }
    });
};


