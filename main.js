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
      c.addEventListener('load'     , function() { t.on_load();      } , true);
      c.addEventListener('message'  , function(a){ t.on_message(a);  } , true);
      c.addEventListener('loadstart', function() { t.on_loadstart(); } , true);
      c.addEventListener('loadend'  , function() { t.on_loadend();   } , true);
      c.addEventListener('progress' , function(a){ t.on_progress(a); } , true);
      c.addEventListener('error'    , function() { t.on_error();     } , true);
      c.addEventListener('abort'    , function() { t.on_abort();     } , true);
      c.addEventListener('crash'    , function() { t.on_crash();     } , true);
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
    on_message: function(a){
      wrp.log_push('on_message',this);
      wrp.log(a.data);
      if(this.message)
        this.message(a);
      wrp.log_pop();
    },
    post_message: function(a){
      wrp.log_push('post_message', this);
      wrp.log(a);
      this.tmp.dom.nacl_target.postMessage(a);
      wrp.log_pop();
    },
    on_loadstart: function(){
      wrp.log_push('on_loadstart', this);
      wrp.log_pop();
    },
    on_loadend: function(){
      wrp.log_push('on_loadend', this);
      wrp.log_pop();
    },
    on_progress: function(a){
      wrp.log_push('on_progress', this);
      var r = a.loaded / a.total;
      this.change_status(1.0 + r);
      if(this.loading)
        this.loading(r);
      wrp.log_pop();
    },
    on_error: function(){
      wrp.log_push('on_error', this);
      if(this.error)
        this.error();
      this.nacl_target_dump();
      wrp.log_pop();
    },
    on_abort: function(){
      wrp.log_push('on_abort', this);
      if(this.abort)
        this.abort();
      this.nacl_target_dump();
      wrp.log_pop();
    },
    on_crash: function(){
      wrp.log_push('on_crash', this);
      if(this.crash)
        this.crash();
      this.nacl_target_dump();
      wrp.log_pop();
    },
    nacl_target_dump: function(){
      wrp.log_push('nacl_target_dump', this);
      var t = this.tmp.dom.nacl_target;
      wrp.log('lastError : ' + t.lastError );
      wrp.log('readyState: ' + t.readyState);
      wrp.log_pop();
    },
  };
  
  wrp_nacl.proc = [];
  return wrp_nacl;
})();

var main = function(){
  var indicate = function(k, v, va, o){
    wrp.log_push('indicate',this);
    wrp.log('k : ' + k );
    wrp.log('v : ' + v );
    wrp.log('va: ' + va);
    wrp.log('o : ' + o );
    var ei = document.getElementById('indicator');
    var ek = document.getElementById('indicator_key');
    var ev = document.getElementById('indicator_value');
    if(typeof o === typeof 0)
      ei.style.opacity = o;
    if(k)
      ek.innerHTML = k;
    if(v)
      ev.innerHTML = v;
    if(va)
      ev.style.textAlign = va;
    wrp.log_pop();
  };
  
  var n = new wrp.nacl();
  //n.message = function(a){};
  n.loading = function(a){
    var o = (a >= 1) ? 0 : 1;
    indicate('LOADING', (a * 100).toFixed(2) + '[%]', 'right', o);
  };
  n.error = function(){ indicate('OOPS!','ERROR','center',1); };
  n.abort = function(){ indicate('OOPS!','ABORT','center',1); };
  n.crash = function(){ indicate('OOPS!','CRASH','center',1); };
  n.initialize();
};

window.addEventListener('load', main, false);

