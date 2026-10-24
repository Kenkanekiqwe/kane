#pragma once
namespace kane::ui {
inline constexpr char kHomePageHtml[] = R"KANEHTML(
<!doctype html><html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><meta name="color-scheme" content="dark"><title>KANE — New Tab</title>
<style>
:root{color-scheme:dark;--bg:#101116;--panel:#191b23;--line:#2b2e39;--text:#f0f1f7;--muted:#969bad;--accent:#9aaeff}
*{box-sizing:border-box}body{margin:0;min-height:100vh;background:radial-gradient(ellipse at 50% 35%,#222637 0%,var(--bg) 58%);color:var(--text);font:15px/1.5 "Segoe UI",sans-serif;display:grid;place-items:center}
main{width:min(680px,calc(100% - 36px));margin-top:-7vh}.brand{display:flex;justify-content:center;align-items:center;gap:13px;margin-bottom:9px}.mark{width:42px;height:42px;border-radius:14px;background:linear-gradient(140deg,#b9c7ff,#7489e8);color:#121522;display:grid;place-items:center;font-size:24px;font-weight:800}h1{font-size:36px;letter-spacing:.12em;margin:0;font-weight:650}.tagline{text-align:center;color:var(--muted);margin:0 0 32px}
form{display:flex;align-items:center;gap:12px;background:var(--panel);border:1px solid var(--line);border-radius:17px;padding:8px 10px 8px 18px;box-shadow:0 18px 60px #0003}form:focus-within{border-color:#879bfa;box-shadow:0 0 0 3px #879bfa20}input{min-width:0;flex:1;border:0;outline:0;background:transparent;color:var(--text);font:inherit;padding:10px 0}button{border:0;border-radius:11px;padding:10px 17px;background:var(--accent);color:#111522;font:600 14px "Segoe UI",sans-serif;cursor:pointer}
h2{font-size:12px;text-transform:uppercase;letter-spacing:.14em;color:var(--muted);font-weight:600;margin:35px 0 12px 3px}.links{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:10px}a{color:inherit;text-decoration:none}.tile{display:flex;align-items:center;gap:12px;min-height:68px;padding:12px 14px;background:#171920b8;border:1px solid #282b35;border-radius:14px;transition:background .15s,border-color .15s}.tile:hover{background:#20232e;border-color:#454b60}.icon{width:34px;height:34px;display:grid;place-items:center;background:#262a37;border-radius:10px;color:#c3ceff;font-weight:700}.name{font-size:13px;font-weight:600}.hint{font-size:11px;color:var(--muted)}footer{text-align:center;color:#666b7c;font-size:12px;margin-top:34px}@media(max-width:520px){.links{grid-template-columns:repeat(2,minmax(0,1fr))}h1{font-size:30px}}
</style></head><body><main><div class="brand"><div class="mark">K</div><h1>KANE</h1></div><p class="tagline">A quieter, lighter place to browse.</p>
<form id="search" autocomplete="off"><span style="color:var(--muted);font-size:20px">⌕</span><input id="query" autofocus placeholder="Search the web or enter an address" aria-label="Search or enter an address"><button type="submit">Go</button></form>
<h2>Quick access</h2><div class="links">
<a class="tile" href="https://www.wikipedia.org"><span class="icon">W</span><span><span class="name">Wikipedia</span><br><span class="hint">Explore knowledge</span></span></a>
<a class="tile" href="https://github.com"><span class="icon">⌘</span><span><span class="name">GitHub</span><br><span class="hint">Projects and code</span></span></a>
<a class="tile" href="https://www.youtube.com"><span class="icon">▷</span><span><span class="name">YouTube</span><br><span class="hint">Watch videos</span></span></a>
<a class="tile" href="https://www.reddit.com"><span class="icon">r/</span><span><span class="name">Reddit</span><br><span class="hint">Communities</span></span></a>
<a class="tile" href="https://www.twitch.tv"><span class="icon">▣</span><span><span class="name">Twitch</span><br><span class="hint">Live streams</span></span></a>
<a class="tile" href="https://duckduckgo.com"><span class="icon">D</span><span><span class="name">DuckDuckGo</span><br><span class="hint">Private search</span></span></a>
</div><footer>Built for focus. Designed to stay out of your way.</footer></main>
<script>document.getElementById('search').addEventListener('submit',function(e){e.preventDefault();const value=document.getElementById('query').value.trim();if(!value)return;const scheme=/^[a-zA-Z][a-zA-Z0-9+.-]*:/.test(value);const address=/^(localhost|[0-9.]+|[^\\s]+\\.[a-zA-Z]{2,})(:\\d+)?(\\/.*)?$/.test(value);location.href=(scheme||address)?(scheme?value:'https://'+value):'https://duckduckgo.com/?q='+encodeURIComponent(value);});</script>
</body></html>
)KANEHTML";
}
