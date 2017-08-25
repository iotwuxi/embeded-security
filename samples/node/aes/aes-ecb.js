const crypto = require('crypto');
const keys = new Buffer.from('000102030405060708090A0B0C0D0E0F', 'hex');
const plaintext = new Buffer.from('000102030405060708090A0B0C0D0E0F', 'hex');
const ciphertext = new Buffer.from('0A940BB5416EF045F1C39458C653EA5A', 'hex');

const cipher = crypto.createCipheriv('aes-128-ecb', keys, '');

var encrypted = cipher.update(plaintext);
// cipher.setAutoPadding(false)
cipher.final();
// encrypted = cipher.final();

console.log("Encrypted Message");
console.log(encrypted);

/*
const decipher = crypto.createDecipheriv('aes-128-ecb', keys, '');
var decrypted = decipher.update(ciphertext);
decrypted = decipher.final()

console.log("Decrypted Message");
console.log(decrypted);
*/