/*
RFC3602

Key       : 0x06a9214036b8a15b512e03d534120006
IV        : 0x3dafba429d9eb430b422da802c9fac41
Plaintext : "Single block msg"
          : 53696e676c6520626c6f636b206d7367
Ciphertext: 0xe353779c1079aeb82708942dbe77181a

Key       : 0xc286696d887c9aa0611bbb3e2025a45a
IV        : 0x562e17996d093d28ddb3ba695a2e6f58
Plaintext : 0x000102030405060708090a0b0c0d0e0f
              101112131415161718191a1b1c1d1e1f
Ciphertext: 0xd296cd94c2cccf8a3a863028b5e1dc0a
              7586602d253cfff91b8266bea6d61ab1
*/

const crypto = require('crypto');
const keys = new Buffer.from('c286696d887c9aa0611bbb3e2025a45a', 'hex');
const iv = new Buffer.from('562e17996d093d28ddb3ba695a2e6f58', 'hex');
const plaintext = new Buffer.from('000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f', 'hex');

const cipher = crypto.createCipheriv('aes-128-cbc', keys, iv);

var encrypted = cipher.update(plaintext)
cipher.final();

console.log(encrypted)
// 输出 <Buffer d2 96 cd 94 c2 cc cf 8a 3a 86 30 28 b5 e1 dc 0a 75 86 60 2d 25 3c ff f9 1b 82 66 be a6 d6 1a b1>
