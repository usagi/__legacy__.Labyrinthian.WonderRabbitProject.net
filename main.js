if (!wrp) var wrp = {};

//WRP_DEBUG_BEGIN
if (!wrp.etc) wrp.etc = {};
//wrp.etc.log_off = true;
if (!wrp.log){
  if (!wrp.etc.log_off) {
    wrp.log = function(a, b){
      var t = wrp.log_stack[wrp.log_stack.length - 1];
      var i = '';
      for(var n = wrp.log_stack.length - 2; n; --n)
        i += '  ';
      console.log(i + '[' + new Date().getTime() + ':' + t.name + '] ', a, t.object);
    };
    wrp.log_push = function(n, o){
      wrp.log_stack.push( {name:n, object:o} );
      wrp.log('>>> push >>>');
    };
    wrp.log_pop = function(){
      wrp.log('<<< pop  <<<');
      wrp.log_stack.pop();
    };
    wrp.log_stack = [{name:void 0,object:void 0}];
  }else
    wrp.log = function(){};
}
//WRP_DEBUG_END

wrp.nacl = (function(){
  var wrp_nacl = function(){
    this.etc = {
      dom: {
        nacl_container: 'nacl_container',
        nacl_target   : 'nacl_target',
      },
    };
    this.tmp = {
      proc  : wrp.nacl.proc.length,
      status: 0,
      dom   : {},
    };
    this.on_message = function(a){
      wrp.log_push('on_message',this);
      wrp.log(a.data);
      wrp.log_pop();
    };
    wrp.nacl.proc.push(this);
  };
  
  wrp_nacl.prototype = {
    initialize: function(){
      wrp.log_push('initialize', this);
      this.initialize_dom();
      this.initialize_event();
      this.initialize_status();
      wrp.log_pop();
    },
    initialize_dom: function(){
      wrp.log_push('initialize_dom', this);
      var ed = this.etc.dom;
      var td = this.tmp.dom;
      td.nacl_container = document.getElementById(ed.nacl_container);
      td.nacl_target    = document.getElementById(ed.nacl_target);
      wrp.log_pop();
    },
    initialize_event: function(){
      wrp.log_push('initialize_event', this);
      var c = this.tmp.dom.nacl_container;
      var t = this;
      c.addEventListener('load'   , function() { t.on_load();     } , true);
      c.addEventListener('message', function(a){ t.on_message(a); } , true);
      wrp.log_pop();
    },
    initialize_status: function(){
      wrp.log_push('initialize_status', this);
      this.change_status(1);
      wrp.log_pop();
    },
    change_status: function(a){
      wrp.log_push('change_status', this);
      wrp.log(this.tmp.status + ' --> ' + a);
      this.tmp.status = a;
      wrp.log_pop();
    },
    on_load: function(){
      wrp.log_push('on_load', this);
      this.tmp.dom.nacl_target =
        document.getElementById(this.etc.dom.nacl_target);
      this.change_status(2);
      wrp.log_pop();
    },
    post_message: function(a){
      this.tmp.dom.nacl_target.postMessage(a);
    },
  };
  
  wrp_nacl.proc = [];
  return wrp_nacl;
})();

var main = function(){
  var n = new wrp.nacl();
  n.initialize();
};

window.addEventListener('load', main, false);

