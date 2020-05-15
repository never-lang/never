<link rel="stylesheet" href="../codemirror.css">
<script src="../codemirror.js"></script>
<script src="../never-cm.js"></script>
<script src="../never-lang.js"></script>

# Never on The Web

<!-- Place this tag in your head or just before your close body tag. -->
<script async defer src="https://buttons.github.io/buttons.js"></script>
<a class="github-button" href="https://github.com/never-lang/never" data-icon="octicon-star" data-size="large" data-show-count="true" aria-label="Star never-lang/never on GitHub">Star me!</a>

You may try Never on the web. Just select an example or write your
own code!

<div>
    <label for="input">Code:</label>
    <textarea id="input" name="input" rows="25" cols="80"></textarea>
</div>

<div>
  <button onclick="runNever()">&#x25b6;</button>
  <select id="snippets" onchange="loadExample()">
    <option value="hello">Hello World!</option>
    <option value="fib">Fibonacci Numbers</option>
    <option value="tsum">Array Sum with Recursion</option>
    <option value="fact">Number Factorization</option>
    <option value="f1">First Class Functions</option>
    <option value="bin">Binary Format</option>
    <option value="rodcut">Dynamic Programming</option>
    <option value="div">Divisors</option>
    <option value="lc">List Comprehension</option>
    <option value="variant">Variants</option>
    <option value="range">Ranges 1</option>
    <option value="range2">Ranges 2</option>
    <option value="trian">Pythagorean Triangle</option>
  </select>
</div>

<div>
    <label for="output">Output:</label>
    <textarea id="output" name="output" rows="25" cols="80" style="font-size:1em; font-family:'Roboto Slab', monospace;"></textarea>
</div>

<div>
&nbsp;
</div>

<script language="javascript">
  var hello_world = 
    "func main() -> int\n" +
    "{\n" +
    "    prints(\"Hello World!\\r\\n\");\n" +
    "    0\n" +
    "}\n";

  var fib = 
    "func fib(n : int) -> int\n"+
    "{\n" +
    "    (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2)\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    fib(20)\n" +
    "}\n";

  var tsum =
    "func tsum( t[elems] : int) -> int\n" +
    "{\n" +
    "    func __tsum( sum : int, i : int, t[elems] : int ) -> int\n" +
    "    {\n" +
    "        i < elems ? __tsum( sum + t[i], i + 1, t ) : sum\n" +
    "    }\n" +
    "    __tsum(0, 0, t)\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    tsum( [ 10, 20, 30, 40, 50, 60 ] : int )\n" +
    "}\n";

   var fact = 
    "func factorial(n : int) -> int\n" +
    "{\n" +
    "    func factrec(n : int, val : int) -> int\n" +
    "    {\n" +
    "        n == 0 ? val : factrec(n - 1, n * val)\n" +
    "    }\n" +
    "\n" +
    "    factrec(n, 1)\n" +
    "}\n" +
    "\n" + 
    "func main() -> int\n" +
    "{\n" +
    "    factorial(3)\n" +
    "}\n";

   var f1 = 
    "func f1(a : int, b : int, c : int) -> [D] : () -> int\n" +
    "{\n" +
    "    [\n" +
    "        let func () -> int { a + b + c }, \n" +
    "        let func () -> int { a + b - c }  \n" +
    "    ] : () -> int\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    f1(80, 90, 100)[1]()\n" +
    "}\n"; 

   var bin =
    "func main() -> int\n" +
    "{\n" +
    "    let n = 32;\n" +
    "\n" +
    "    do\n" +
    "    {\n" +
    "        print(n % 2);\n" +
    "        n = n / 2\n" +
    "    } while (n != 0)\n" +
    "}\n";

   var rodcut =
    "func max(a : int, b : int) -> int\n" +
    "{\n" +
    "    a > b ? a : b\n" +
    "}\n" +
    "\n" +
    "func cutrod(price[P] : int, memo[M] : int, len : int) -> int\n" +
    "{\n" +
    "    var i = 0;\n" +
    "    var max_p = -1;\n" +
    "\n" +
    "    if (memo[len] != -1)\n" +
    "    {\n" +
    "        max_p = memo[len]\n" +
    "    }\n" +
    "    else\n" +
    "    {\n" +
    "         while (i < len)\n" +
    "         {\n" +
    "             max_p = max(max_p, price[i] + cutrod(price, memo, len - i - 1));\n" +
    "             i = i + 1\n" +
    "         }\n" +
    "    };\n" +
    "    \n" +
    "   memo[len] = max_p\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] : int;\n" +
    "    let memo = [ 0, -1, -1, -1, -1, -1, -1, -1, -1 ] : int; \n" +
    "    \n" +
    "    cutrod(price, memo, 8)\n" +
    "}\n";

    var div =
    "func divisors(n : int) -> int\n" +
    "{\n" +
    "    var i = 1;\n" +
    " \n" +
    "    for (i = 1; i * i <= n; i = i + 1) \n" +
    "    {\n" +
    "        if (n % i == 0)\n" +
    "        {\n" +
    "            if (n / i != i)\n" +
    "            {\n" +
    "                print(n / i);\n" +
    "                print(i)\n" +
    "            }\n" +
    "            else\n" +
    "            {\n" +
    "                print(i)\n" +
    "            }\n" +
    "        }\n" +
    "    }\n" +
    "\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    divisors(60)\n" +
    "}\n";

    var lc =
    "func print_cl(cl[D] : int) -> int\n" +
    "{\n" +
    "    var i = 0;\n" +
    "\n" +
    "    for (i = 0; i < D; i = i + 1)\n" +
    "    {\n" +
    "        print(cl[i])\n" +
    "    };\n" +
    "\n" +
    "    0\n" +
    "}\n" +
    "\n" +
    "func cl() -> [_] : int\n" +
    "{\n" +
    "    [ x * x | x in [10, 20, 30, 40, 50] : int ] : int\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "   print_cl(cl())\n" +
    "}\n";

    var variant =
    "enum Variant { Int { value : int; },\n" +
    "               Float { value : float; },\n" +
    "               Char { value : char; },\n" +
    "               String { value : string; } }\n" +
    "\n" +
    "func printv ( v : Variant ) -> Variant\n" +
    "{\n" +
    "    match (v)\n" +
    "    {\n" +
    "        Variant::Int(value) -> { print(value); v };\n" +
    "        Variant::Float(value) -> { printf(value); v };\n" +
    "        Variant::Char(value) -> { printc(value); v };\n" +
    "        Variant::String(value) -> { prints(value); v };\n" +
    "    }\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    let i = 10;\n" +
    "    let f = 10.0;\n" +
    "    let c = 'A';\n" +
    "\n" +
    "    printv(Variant::Int(i));\n" +
    "    printv(Variant::Float(f));\n" +
    "    printv(Variant::Char(c)); prints(\"\\r\\n\");\n" +
    "    printv(Variant::String(\"ten\")); prints(\"\\r\\n\");\n" +
    "\n" +
    "    0\n" +
    "}\n";

    var range =
    "func main() -> int\n" +
    "{\n" +
    "    let hw = \"Hello World!\";\n" +
    "    prints(hw[length(hw)-1 .. 0] + \"\\r\\n\");\n" +
    "    0\n" +
    "}\n";

    var range2 =
    "func pr_range( [ from .. to ] : range ) -> int\n" +
    "{\n" +
    "    prints(\"range [\" + from + \"..\" + to + \"]\\r\\n\");\n" +
    "    0\n" +
    "}\n" +
    "\n" +
    "func pr_ranges( ranges[D] : () -> [..] : range ) -> int\n" +
    "{\n" +
    "    for (gr in ranges)\n" +
    "    {\n" +
    "        pr_range(gr())\n" +
    "    }\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    let r = [  let func get_range() -> [..] : range\n" +
    "                {\n" +
    "                    [ from .. to ]\n" +
    "                }\n" +
    "                |\n" +
    "               from in [ 0 .. 10 ];\n" +
    "                to in [ 0 .. 10 ]\n" +
    "            ] : () -> [..] : range;\n" +
    "\n" +
    "    pr_ranges(r);\n" +
    "\n" +
    "    0\n" +
    "}\n";

   var trian = 
    "record Triangle\n" +
    "{\n" +
    "    a : int;\n" +
    "    b : int;\n" +
    "    c : int;\n" +
    "}\n" +
    "\n" +
    "func printTriangle(t[D] : Triangle) -> int\n" +
    "{\n" +
    "    for (d in t)\n" +
    "        prints(\"pythagorean triangle\" +\n" +
    "               \" a = \" + d.a + \n" +
    "               \" b = \" + d.b + \n" +
    "               \" c = \" + d.c + \"\\r\\n\")\n" +
    "}\n" +
    "\n" +
    "func getRange() -> [..] : range\n" +
    "{\n" +
    "    [ 1..13 ]\n" +
    "}\n" +
    "\n" +
    "func main() -> int\n" +
    "{\n" +
    "    let r = getRange();\n" +
    "    let pitria = [ Triangle(a, b, c) |\n" +
    "                     a in r;\n" +
    "                     b in r;\n" +
    "                     c in r;\n" +
    "                     a * a + b * b == c * c ] : Triangle;\n" +
    "\n" +
    "    printTriangle(pitria);\n" +
    "\n" +
    "    0\n" +
    "}\n";

  var snippets = {
      "hello": hello_world,
      "fib": fib,
      "tsum": tsum,
      "fact": fact,
      "f1": f1,
      "bin": bin,
      "rodcut": rodcut,
      "div": div,
      "lc": lc,
      "variant": variant,
      "range": range,
      "range2": range2,
      "trian": trian
  }

  function loadExample() {
    var select = document.getElementById('snippets');
    editor.setValue(snippets[select.value]);
  }

  function runNever() {
    var output = document.getElementById("output");

    output.value = "";
    ret = never(editor.getValue());
    output.value += "\r\nresult: ";
    output.value += ret;
  }

  var editor = CodeMirror.fromTextArea(document.getElementById("input"), {
    lineNumbers: true,
    indentUnit: 4
  });

  var never = Module.cwrap('never','number',['string']);
  
  editor.setValue(hello_world);
</script>


