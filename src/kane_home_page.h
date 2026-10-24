#pragma once
namespace kane::ui {
inline constexpr char kHomePageHtml[] = R"KANEHTML(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="color-scheme" content="dark">
<title>KANE — New Tab</title>
<style>
:root{color-scheme:dark;--bg:#151619;--panel:#202125;--panel2:#25262b;--line:#303136;--text:#f2f2f4;--muted:#9b9ca5;--accent:#c4c8ff;--accent-ink:#171821;--rail:68px}
*{box-sizing:border-box}
body{margin:0;min-height:100vh;background:var(--bg);color:var(--text);font:14px/1.5 Inter,"Segoe UI",sans-serif}
.rail{position:fixed;inset:0 auto 0 0;width:var(--rail);background:#191a1d;border-right:1px solid #292a2e;display:flex;align-items:center;flex-direction:column;padding:17px 0;gap:11px}
.logo{width:37px;height:37px;display:grid;place-items:center;border-radius:13px;background:#c4c8ff;color:#171821;font-size:20px;font-weight:800;margin-bottom:24px}
.rail-btn{width:42px;height:42px;display:grid;place-items:center;border-radius:13px;color:#9b9ca5;text-decoration:none;font-size:18px;border:1px solid transparent;transition:.15s}
.rail-btn:hover,.rail-btn.active{background:#292a30;color:#f2f2f4;border-color:#35363d}
.rail-bottom{margin-top:auto}
main{margin-left:var(--rail);min-height:100vh;display:flex;flex-direction:column;align-items:center;padding:12vh 28px 32px}
.hero{width:min(720px,100%);text-align:center}
.wordmark{font-size:42px;font-weight:680;letter-spacing:.19em;margin:0 0 4px 7px}
.tagline{margin:0 0 35px;color:var(--muted);font-size:14px}
.search-wrap{background:#222327;border:1px solid #34353b;border-radius:18px;padding:7px;display:flex;align-items:center;gap:5px;box-shadow:0 16px 45px #0002;transition:border-color .15s,box-shadow .15s}
.search-wrap:focus-within{border-color:#686b7e;box-shadow:0 0 0 3px #c4c8ff0c,0 16px 45px #0002}
.search-symbol{padding:0 8px 0 11px;color:#a7a8b1;font-size:21px}
input{flex:1;min-width:0;background:transparent;border:0;outline:0;color:var(--text);font:inherit;padding:12px 2px}
input::placeholder{color:#92939d}
select{appearance:none;max-width:145px;background:#2b2c32;color:#d6d7df;border:1px solid #3a3b42;border-radius:11px;padding:9px 26px 9px 11px;font:12px "Segoe UI",sans-serif;outline:0;cursor:pointer;background-image:linear-gradient(45deg,transparent 50%,#a9aab4 50%),linear-gradient(135deg,#a9aab4 50%,transparent 50%);background-position:calc(100% - 12px) 15px,calc(100% - 8px) 15px;background-size:4px 4px;background-repeat:no-repeat}
.go{width:39px;height:39px;border:0;border-radius:12px;background:var(--accent);color:var(--accent-ink);font-size:19px;cursor:pointer;flex:none}
.go:hover{filter:brightness(1.08)}
.section-head{display:flex;align-items:center;justify-content:space-between;margin:35px 0 13px;text-align:left}
.section-head h2{font-size:12px;font-weight:650;letter-spacing:.12em;text-transform:uppercase;color:#a4a5af;margin:0}
.section-head span{font-size:12px;color:#797a84}
.tiles{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:10px;text-align:left}
.tile{min-height:76px;display:flex;align-items:center;gap:12px;padding:13px;background:#1e1f23;border:1px solid #2e2f34;border-radius:15px;color:inherit;text-decoration:none;transition:background .15s,border-color .15s,transform .15s}
.tile:hover{background:#26272c;border-color:#42434b;transform:translateY(-1px)}
.site-icon{width:35px;height:35px;display:grid;place-items:center;background:#2a2b31;border:1px solid #36373e;border-radius:11px;color:#d1d3ff;font-size:15px;font-weight:700;flex:none}
.site-name{font-size:13px;font-weight:600}.site-hint{font-size:11px;color:#92939d;margin-top:2px}
.bottom{margin-top:auto;padding-top:48px;font-size:11px;color:#686973}
@media(max-width:650px){:root{--rail:54px}.rail{width:var(--rail)}main{padding:10vh 15px 24px}.wordmark{font-size:34px}.tiles{grid-template-columns:repeat(2,minmax(0,1fr))}.search-wrap{flex-wrap:wrap}.search-symbol{padding-left:7px}select{max-width:125px}}
</style>
</head>
<body>
<aside class="rail" aria-label="KANE navigation">
<div class="logo" title="KANE">K</div>
<a class="rail-btn active" href="#" title="New tab" aria-label="New tab">⌂</a>
<a class="rail-btn" href="https://www.wikipedia.org" title="Wikipedia" aria-label="Wikipedia">◈</a>
<a class="rail-btn" href="https://github.com" title="GitHub" aria-label="GitHub">⌘</a>
<a class="rail-btn" href="https://www.youtube.com" title="YouTube" aria-label="YouTube">▷</a>
<a class="rail-btn rail-bottom" href="#settings" title="Search settings" aria-label="Search settings" id="settingsLink">⚙</a>
</aside>
<main>
<section class="hero">
<h1 class="wordmark">KANE</h1>
<p class="tagline">Your space. Your pace.</p>
<form id="search" class="search-wrap" autocomplete="off">
<span class="search-symbol" aria-hidden="true">⌕</span>
<input id="query" autofocus placeholder="Search or enter a website" aria-label="Search or enter a website">
<select id="engine" aria-label="Search engine" title="Choose search engine">
<option value="brave">Brave Search</option>
<option value="google">Google</option>
<option value="bing">Bing</option>
<option value="startpage">Startpage</option>
<option value="ecosia">Ecosia</option>
<option value="duckduckgo">DuckDuckGo</option>
</select>
<button class="go" type="submit" aria-label="Search">↗</button>
</form>
<div class="section-head"><h2>Shortcuts</h2><span>Quick access</span></div>
<div class="tiles">
<a class="tile" href="https://www.wikipedia.org"><span class="site-icon">W</span><span><span class="site-name">Wikipedia</span><div class="site-hint">Knowledge and ideas</div></span></a>
<a class="tile" href="https://github.com"><span class="site-icon">⌘</span><span><span class="site-name">GitHub</span><div class="site-hint">Code and projects</div></span></a>
<a class="tile" href="https://www.youtube.com"><span class="site-icon">▷</span><span><span class="site-name">YouTube</span><div class="site-hint">Videos and music</div></span></a>
<a class="tile" href="https://www.reddit.com"><span class="site-icon">r/</span><span><span class="site-name">Reddit</span><div class="site-hint">Communities</div></span></a>
<a class="tile" href="https://www.twitch.tv"><span class="site-icon">▣</span><span><span class="site-name">Twitch</span><div class="site-hint">Live streams</div></span></a>
<a class="tile" href="https://www.wikipedia.org/wiki/Special:Random"><span class="site-icon">✳</span><span><span class="site-name">Discover</span><div class="site-hint">Something new</div></span></a>
</div>
</section>
<div class="bottom">Minimal by design · KANE Browser</div>
</main>
<script>
(function(){
const engines={
brave:q=>'https://search.brave.com/search?q='+encodeURIComponent(q),
google:q=>'https://www.google.com/search?q='+encodeURIComponent(q),
bing:q=>'https://www.bing.com/search?q='+encodeURIComponent(q),
startpage:q=>'https://www.startpage.com/sp/search?query='+encodeURIComponent(q),
ecosia:q=>'https://www.ecosia.org/search?q='+encodeURIComponent(q),
duckduckgo:q=>'https://duckduckgo.com/?q='+encodeURIComponent(q)
};
const input=document.getElementById('query'),select=document.getElementById('engine');
try{const saved=localStorage.getItem('kane.searchEngine');if(saved&&engines[saved])select.value=saved;}catch(_){}
select.addEventListener('change',()=>{try{localStorage.setItem('kane.searchEngine',select.value)}catch(_){}});
document.getElementById('settingsLink').addEventListener('click',()=>{select.focus();select.click()});
document.getElementById('search').addEventListener('submit',function(e){
e.preventDefault();const value=input.value.trim();if(!value)return;
const scheme=/^[a-zA-Z][a-zA-Z0-9+.-]*:/.test(value);
const address=value==='localhost'||/^[^\s]+\.[a-zA-Z]{2,}(:\d+)?(\/.*)?$/.test(value)||/^\d{1,3}(\.\d{1,3}){3}(:\d+)?$/.test(value);
location.href=(scheme||address)?(scheme?value:'https://'+value):engines[select.value](value);
});
})();
</script>
</body>
</html>
)KANEHTML";
}
