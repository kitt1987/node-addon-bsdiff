# node-addon-bsdiff

bsdiff & bspatch for Node. Thanks to [minibsdiff](https://github.com/thoughtpolice/minibsdiff).

I dont wanna publish it to NPM cause it is just a wrapper of minibsdiff, not a true bsdiff in pure Javascript. And there are too many bsdiff modules on NPM though most of them doesnt work any more.
So, try the follow way to install it or just download sources.

## Installation

`npm i --save git+https://git@github.com/kitt1987/node-addon-bsdiff.git#master`

## HowTo
```
const bs = require('node-addon-bsdiff');
const patch = bs.diff(oldBufferOrString, newBufferOrString);
console.log(Buffer.isBuffer(patch)); // true. If old and new are the same, null will return.

const newVersion = bs.patch(oldBufferOrString, patchBufferOrString);
console.log(Buffer.isBuffer(newVersion)); // true
```

## License
2-Clause BSD