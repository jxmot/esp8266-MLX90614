// load the google gauge visualization - google API load 
google.load('visualization', '1', {packages:['gauge']});
google.setOnLoadCallback(window.initGauges);

// display the data
function displayData(ix, point) {
    console.log('displayData() - ix = ' + ix + '  point = ' + point);
    if(ix >= 0 && ix < gauge_cfg.length) {
        gauge_cfg[ix].data.setValue(0, 0, gauge_cfg[ix].name);
        gauge_cfg[ix].data.setValue(0, 1, point);
        gauge_cfg[ix].chart.draw(gauge_cfg[ix].data, gauge_cfg[ix].opt);
    } else console.log('displayData() - ix out of range = ' + ix);
};


// initialize the guages...
function initGauges() {
    for(var ix = 0; ix < gauge_cfg.length; ix++)
    {
        gauge_cfg[ix].data = new google.visualization.DataTable();
        gauge_cfg[ix].data.addColumn('string', 'Label');
        gauge_cfg[ix].data.addColumn('number', 'Value');
        gauge_cfg[ix].data.addRows(1);

        gauge_cfg[ix].chart = new google.visualization.Gauge(document.getElementById(gauge_cfg[ix].target));

        if('thingspeak' === gauge_cfg[ix].data_source) {
            window.thingspk_loadData(ix);
            setInterval(window.thingspk_loadData, thingspk_cfg.interval, ix);
        }
    }
};


