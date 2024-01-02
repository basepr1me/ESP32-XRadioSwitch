/*
 * Copyright (c) 2023 Tracey Emery K7TLE
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<style>
		html {
			font-family:Helvetica;
			display: inline-block;
			margin: 0px auto;
			text-align: center;
		}
		p {
			font-size: 1.2rem;
		}
		body {
			margin: 0;
		}
		.topbar {
			overflow: hidden;
			background-color: #000077;
			color: white;
			font-size: 1rem;
		}
		.content {
			padding: 20px;
		}
		.card {
			background-color: white;
			box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
		}
		.cards {
			max-width: 300px;
			margin: 0 auto;
			display: grid;
			grid-gap: 2rem;
			grid-template-columns: repeat(auto-fit,
			    minmax(200px, 1fr));
		}
	        .status {
			font-size: 1.4rem;
		}
		button {
			border: 1px solid #0066cc;
			background-color: #0099cc;
			color: #ffffff;
			padding: 5px 10px;
		}
		button:hover {
			border: 1px solid #0099cc;
			background-color: #00aacc;
			color: #ffffff;
			padding: 5px 10px;
		}
		button:onpress {
			border: 1px solid #0099cc;
			background-color: #ff0000;
			color: #ffffff;
			padding: 5px 10px;
		}
		button:disabled,
		button[disabled] {
			border: 1px solid #999999;
			background-color: #cccccc;
			color: #666666;
		}
	</style>
	<script>
		var xhr;

		if (!!window.EventSource) {
			var source = new EventSource("/event");
			source.addEventListener("reload", function (e) {
				location.reload();
			}, false);
		}
		function r_0(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?radio=0", false);
			xhr.send();
		}
		function r_1(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?radio=1", false);
			xhr.send();
		}
		function r_2(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?radio=2", false);
			xhr.send();
		}
		function a_1(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?ant=1", false);
			xhr.send();
		}
		function a_2(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?ant=2", false);
			xhr.send();
		}
		function a_3(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?ant=3", false);
			xhr.send();
		}
		function a_4(element) {
			xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?ant=4", false);
			xhr.send();
		}
	</script>
</head>
<body>
	<div class="topbar">
		<h1>XRadioSwitch</h1>
	</div>
	<div class="content">
		<div class="cards">
			<div class="card">
				<p><b>SELECTIONS</b></p>
				<p><span class="status">
					<span id="selection">%SELECTION%</span>
				</span></p>
			</div>
			<div class="card">
				<p>RADIOS</p>
				<p><span class="status">
					<button id="radio1" onClick="r_1(this)">RADIO 1</button>
					<button id="radio2" onClick="r_2(this)">RADIO 2</button>
					<button id="radio0" onClick="r_0(this)">DUMMY LOAD</button>
				</span></p>
			</div>
			<div class="card">
				<p>ANTENNAS</p>
				<p><span class="status">
					<button id="ant1" onClick="a_1(this)">ANTENNA 1</button>
					<button id="ant2" onClick="a_2(this)">ANTENNA 2</button>
					<button id="ant3" onClick="a_3(this)">ANTENNA 3</button>
					<button id="ant4" onClick="a_4(this)">ANTENNA 4</button>
				</span></p>
			</div>
</body>
</html>
)rawliteral";
