const crypto = require('crypto');
const hash = crypto.createHash('md5');

hash.update('Hello, world!');
console.log(hash.digest('hex'));
