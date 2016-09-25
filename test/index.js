'use strict';

var bs = require('..');

module.exports = {
	diffTheSame: t => {
		var dataA = 'abcdef', dataB = 'abcdef';
		var diff = bs.diff(dataA, dataB);
		t.nothing(diff);
		t.done();
	}
}