const crypto = require('crypto');

var plaintext = '12345678901234567890';
var key = new Buffer.from([0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b,
                        0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06]);
var iv = new Buffer.from([0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30,
                        0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41]);

console.log(key);
console.log(iv);

const cipher = crypto.createCipheriv('aes-128-cbc', key, iv);
var ciphertext = cipher.update(plaintext, 'utf-8', 'hex');
ciphertext += cipher.final('hex');

console.log(new Buffer.from(ciphertext, 'hex'))
// result
// <Buffer e7 b4 cf 6b 1a 4a e6 77 c3 1a 97 ee 35 45 e9 39 b8 3b 92 83 2c e4 3e ed f4 d5 5a 74 3e c9 63 d0>