var gauge_cfg = [
    // 
    {
        target: 'gauge_div',
        name:'Object',
        type: 'T',                      // or 'H'
        unit: 'F',                      // or 'C' or '%' <-- get from incoming data, req's mods
        data_source: 'thingspeak',      // or 'firebase' or ????
        data_channel: 'field2',         // the thingspeak field, or use a unique ID, this could be "name" or "loc"
        round: true,                    // true = round the value before displaying it
        opt: {
            min: 25, max: 120,          // the range of the gauge
            width: 180, height: 180,    // size
            yellowColor: 'blue',        // change the color of the defined `yellow` sector
            yellowFrom:25, yellowTo: 55,
            greenFrom: 55, greenTo: 80,
            redFrom: 80, redTo: 120,
            minorTicks: 5
        },
        chart: {},
        data: {}
    },
    // 
    {
        target: 'gauge_div2',
        name:'Ambient',
        type: 'T',                      // or 'H'
        unit: 'F',                      // or 'C' or '%' <-- get from incoming data, req's mods
        data_source: 'thingspeak',      // or 'firebase' or ????
        data_channel: 'field1',         // the thingspeak field, or use a unique ID, this could be "name" or "loc"
        round: false,                   // true = round the value before displaying it
        opt: {
            min: 25, max: 120, 
            width: 180, height: 180,
            yellowColor: 'blue',
            yellowFrom:25, yellowTo: 55,
            greenFrom: 55, greenTo: 80,
            redFrom: 80, redTo: 120,
            minorTicks: 5
        },
        chart: {},
        data: {}
    }
];
