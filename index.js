'use strict';

function requireAddon() {
  try {
    return require('./build/Release/bsdiff_addon.node');
  } catch (err) {
    return require('./build/Debug/bsdiff_addon.node');
  }
}

const bs = requireAddon();
const crypto = require('crypto');

function digest(data) {
	var hasher = crypto.createHash('md5');
	hasher.update(data);
	return hasher.digest('hex');
}

function diff(a, b) {
	if (digest(a) === digest(b)) return;
	return bs.diff(a, b);
}

function patch(a, p) {
	if (!p) return a;
	return bs.patch(a, p);
}

module.exports = {
	diff,
	patch,
	rawDiff: bs.diff,
	rawPatch: bs.patch,
}
