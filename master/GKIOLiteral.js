var GKIOLiteral = {};

GKIOLiteral.Int32 = function (value) {
    if (!(this instanceof GKIOLiteral.Int32)) return new GKIOLiteral.Int32(value);
    this.value = parseInt(value) || 0;
};

GKIOLiteral.Int32.prototype.toBuffer = function () {
    var res = Buffer.alloc(4);
    res.writeInt32LE(this.value, 0);
    return res;
};
GKIOLiteral.Int32.size = 4;
GKIOLiteral.Int32.fromBuffer = function (buffer, offset) {
    offset = offset || 0;
    return new GKIOLiteral.Int32(buffer.readInt32LE(offset));
};


GKIOLiteral.Int64 = function (value) {
    if (!(this instanceof GKIOLiteral.Int64)) return new GKIOLiteral.Int64(value);
    this.value = parseInt(value) || 0;
};

GKIOLiteral.Int64.prototype.toBuffer = function () {
    var res = Buffer.alloc(8);
    res.writeIntLE(this.value, 0, 8);
    return res;
};
GKIOLiteral.Int64.size = 8;
GKIOLiteral.Int64.fromBuffer = function (buffer, offset) {
    offset = offset || 0;
    return new GKIOLiteral.Int64(buffer.readIntLE(offset, 8));
};



GKIOLiteral.CString = function (size) {
    size = size || 256;
    GKIOLiteral.CString['' + size] = function (value) {
        if (!(this instanceof GKIOLiteral.CString['' + size])) return new GKIOLiteral.CString['' + size](value);
        this.value = value;
    }
    GKIOLiteral.CString['' + size].size = size;


    GKIOLiteral.CString['' + size].prototype.toBuffer = function () {
        return Buffer.from(this._value + '\0');
    };
    GKIOLiteral.CString['' + size].fromBuffer = function (buffer, offset) {
        offset = offset || 0;
        var newBuffer = buffer.slice(offset, size);
        var i = 0;
        while (newBuffer[i] != 0)++i;
        newBuffer = newBuffer.slice(0, i);
        var s = newBuffer.toString();
        return new GKIOLiteral.CString['' + size](s);
    };

    Object.defineProperty(GKIOLiteral.CString['' + size].prototype, 'value', {
        set: function (value) {
            this._value = value + '' || '';
            while (this._value.length < size) {
                this._value += '\0';
            }
        }, get: function () {
            return this._value.replace(/[\0]+/, '');
        }
    });
    return GKIOLiteral.CString['' + size];
};


module.exports = GKIOLiteral;