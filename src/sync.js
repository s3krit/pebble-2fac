Pebble.addEventListener('ready',
  function(e) {
    // Probably the right place to sync keys
    console.log('JavaScript app ready and running!');
    var d = new Date();
    var offset = d.getTimezoneOffset()*60;
    var configuration = {Timezone_offset: offset};
    console.log(configuration.Timezone_offset);
    Pebble.sendAppMessage(configuration);
  }
);

Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
    Pebble.openURL('https://pebble-config.herokuapp.com/config?title=Two-Step%20Configuration&fields=text_Secret');
});

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(e.response);
    Pebble.sendAppMessage(configuration);
  }
);