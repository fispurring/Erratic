var page=require('webpage').create();
var system=require('system');
var child_process=require('child_process');
var fs=require('fs');

if(system.args.length===1) {
    phantom.exit();
}

var url=system.args[1];
page.settings.userAgent='Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.65 Safari/537.36';
page.viewportSize={width:800,height:720};
console.log('ViewportSize:'+JSON.stringify(page.viewportSize));

function ScrollDown(curPage,totalPage) {
    window.setTimeout(function(){
	page.scrollPosition = { top: page.scrollPosition.top + 720, left: 0 };
	curPage+=1;
	if(curPage<=totalPage) {
	    ScrollDown(curPage,totalPage);
	}
    },300);
}

function ScrapeBaidu() {
    return page.evaluate(function() {
	var reg=/&quot;purl&quot;\s*:\s*&quot;(((?!&quot;)\S)+)/gim;
	var result=[];
	var r;
	while(r=reg.exec(document.all[0].innerHTML)) {
	    result.push(r[1]);
	}
	return result;
    });
}

function TryDownload(curCount,totalCount,pResult,dirname) {
    var result=pResult.slice(0);
    var url=result.pop();
    var type=url.match(/.\w+$/)[0];
    child_process.execFile('curl',['-o',dirname+'/'+curCount+type,url],null,function(err,stdout,stderr) {
	curCount+=1;
	console.log('Process:'+curCount+'/'+totalCount);
	console.log(stderr);
	if(result.length>0) {
	    window.setTimeout(function() {TryDownload(curCount,totalCount,result,dirname);},500);
	}
	else {
	    phantom.exit();
	}
    });
}

page.open(url,function(status) {
    if(status !== 'success') {
	console.log('Unable to load url.');
	phantom.exit();
    }
    var pageCount=20;
    ScrollDown(1,pageCount);

    window.setTimeout(function() {
	console.log('begin');
	var result=ScrapeBaidu();
	console.log('end');
	if(result.length>0) {
	    var date=new Date();
	    var dirname=""+date.getFullYear()+(date.getMonth()+1)+date.getDate()+date.getHours()+date.getMinutes()+date.getSeconds();
	    if(!fs.exists(dirname)) {
		fs.makeDirectory(dirname);
	    }
	    TryDownload(0,result.length,result,dirname);
	}
	else {
	    console.log('Can\'t sniff image url.')
	}	
    },300*pageCount+3000);
});

