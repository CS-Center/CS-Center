class wcics_promise {
  constructor(promise) {
    this.response = undefined;
    this.good_funcs = [];
    this.catch_funcs = [];
    promise.then(r => {
      r.text().then(t => {
        this.response = r;
        if(r.ok) {
          for(var i in this.good_funcs) {
            this.good_funcs[i](t, r.status, r);
          }
        }
        else {
          for(var j in this.catch_funcs) {
            var status_code = this.catch_funcs[j][1];
            if(!status_code || (status_code < 10 && Math.floor(r.status / 100) == status_code) || status_code == r.status) {
              this.catch_funcs[j][0](t, r.status, r);
            }
          }
        }
      });
    });
  }
  
  then(f) {
    if(this.response && this.response.ok) {
      f();
    }
    this.good_funcs.push(f);
    return this;
  }
  
  // status_code can be:
  // - undefined / null / 0 ( a falsy value )
  //   - Means that it will execute for all errors.
  // - a 1 digit number
  //   - 4 will execute for all 4xx status codes
  // - a 3 digit number, the complete status code.
  
  catch(status_code, f) {
    // If f is not defined, then status_code contains a function to be run for all errors.
    if(!f) {
      f = status_code;
      status_code = false;
    }
    if(this.response && (!status_code || (status_code < 10 && Math.floor(this.reponse.status / 100) == status_code) || status_code == this.response.status)) {
      f();
    }
    this.catch_funcs.push([f, status_code]);
    return this;
  }
}

function GET(url) {
  return new wcics_promise(fetch(url));
}

function POST(url, data, callback, catchfunc) {
  data = data || {};
  return new wcics_promise(fetch(url, {
    method: "POST",
    headers: {
      "Accept": "application/json",
      "Content-Type": "application/json"
    },
    body: JSON.stringify(data)
  }));
}

// https://stackoverflow.com/a/6234804/8200485
function escapeHtml(unsafe) {
  return unsafe
       .replace(/&/g, "&amp;")
       .replace(/</g, "&lt;")
       .replace(/>/g, "&gt;")
       .replace(/"/g, "&quot;")
       .replace(/'/g, "&#039;");
}