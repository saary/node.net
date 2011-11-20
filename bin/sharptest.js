var SharpObject = require('./Sharp').SharpObject;
var so = new SharpObject();
console.log(so.getSharpValue());

so.async("http://www.google.com", function(err, data) {
    if (err) {
       console.log("Error:", err);
       return;
    }

    console.log("Data:", data);
});