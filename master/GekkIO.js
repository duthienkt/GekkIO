const { spawn } = require('child_process');
const EventEmitter = require('events');
class GekkIO extends EventEmitter {
    constructor(props) {
        super();
        if (typeof (props) == 'string') props = { script: props, debug: false };
        this.props = props;
        this.debug = props.debug;
        this.process = spawn(props.script);
        this.isRunning = true;
        this.process.stdout.on('data', data => this._process(data.toString()));
        this.process.on('exit', code => {
            this.isRunning = false;
            this.emit('exit', code)
        });

        this._eventName = '';
        this._eventData = '';
        this._reading = -1;
    }


    send(name, data) {
        if (this.debug) console.log(`_send_<[${name}](${data.toString('base64')})\n>`);
        this.process.stdin.write(`[${name}](${data.toString('base64')})\n`);
    }

    dispose() {
        if (!this.isRunning) return;
        this.process.stdin.pause();
        this.process.kill();
        this.isRunning = false;
    }

    _process(s) {
        if (this.debug) console.log('_recive_<' + s + '>');
        var c;
        for (var i = 0; i < s.length; ++i) {
            c = s[i];
            switch (c) {
                case '[':
                    this._reading = 0;
                    break;
                case ']':
                    this._reading = 1;
                    break;
                case '(':
                    this._reading = 2;
                    break
                case ')':
                    this._reading = 3;
                    this.emit(this._eventName, Buffer.from(this._eventData, 'base64'));
                    this._eventName = '';
                    this._eventData = '';
                    break;
                default:
                    switch (this._reading) {
                        case 0:
                            this._eventName += c;
                            break;
                        case 2:
                            this._eventData += c;
                            break;
                    }
                    break;
            }
        }
    }

}

module.exports = GekkIO;