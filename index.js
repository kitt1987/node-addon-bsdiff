'use strict';

function requireAddon() {
  try {
    return require('./build/Release/bsdiff_addon.node');
  } catch (err) {
    return require('./build/Debug/bsdiff_addon.node');
  }
}

const bsdiff = requireAddon();

module.exports = {
	diff: bsdiff.diff,
	patch: bsdiff.patch
}
