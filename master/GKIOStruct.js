function GKIOStruct() {
    if (!(this instanceof GKIOStruct)) return new GKIOStruct();
    this.properties = [];
}


GKIOStruct.prototype.property = function (type, key) {
    this.properties.push({ type, key });
    return this;
};




GKIOStruct.prototype.declare = function () {
    var properties = this.properties;
    var res = function (params) {
        if (!(this instanceof res)) return new res(params);

        params = params || {};
        properties.forEach(prop => {
            this[prop.key] = params[prop.key];
        });

    };
    properties.forEach(prop => {
        Object.defineProperty(res.prototype, prop.key, {
            set: function (value) {
                if (!value) this['__' + prop.key] = new prop.type();
                else {
                    if (value instanceof prop.type) {
                        this['__' + prop.key] = value;
                    }
                    else
                        this['__' + prop.key] = new prop.type(value);
                }
            },
            get: function () {
                return this['__' + prop.key];
            }
        });
    });

    res.prototype.toBuffer = function () {
        return Buffer.concat(properties.map(prop => this['__' + prop.key].toBuffer()));
    };
    
    res.fromBuffer = function (buffer, offset) {
        offset = offset||0;
        var params = properties.reduce((ac, prop) => {
            ac[prop.key] = prop.type.fromBuffer(buffer, offset);
            offset += prop.type.size;
            return ac;
        }, {});
        return new res(params);
    };

    res.size = properties.reduce((ac, prop) => ac + prop.type.size, 0);
    return res;
};

module.exports = GKIOStruct;