function LoadPage()
{
    document.getElementById("mainFrame").innerHTML = '<object type="text/html" data="main.html"><script>SetTestText("test")</script></object>';
}

document.addEventListener("DOMContentLoaded", function(event) 
{ 
    LoadPage();
});

