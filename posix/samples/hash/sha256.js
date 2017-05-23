const crypto = require('crypto');
const hash = crypto.createHash('sha256');

hash.update('Hello, world!');
console.log(hash.digest('hex'));
