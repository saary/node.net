var SharpObject = require('./Sharp').SharpObject;
var so = new SharpObject();
console.log(so.getSharpValue());

so.async2("http://anode.sys.anodejs.org/", function (err, data) {
    if (err) {
       console.log("Error:", err);
       return;
    }

    console.log("Data:", data);
});

//setInterval(function() { console.log("."); }, 1000);