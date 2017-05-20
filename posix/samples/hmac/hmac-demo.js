const crypto = require('crypto');
const hmac = crypto.createHmac('sha256', 'a secret');

hmac.update('some data to hash');
console.log(hmac.digest('hex'));
// Prints:
// 7fd04df92f636fd450bc841c9418e5825c17f33ad9c87c518115a45971f7f77e
