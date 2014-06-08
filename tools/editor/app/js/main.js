var fs = require('fs');
var path = require('path');

//var root = path.dirname(process.execPath);

var root = path.resolve("../..");
console.log(root);
var getExtension = function(filename)
{
    var extension = filename.split('.').pop();
    if(extension == filename) return "";
    return extension;
}

var getFilesFromPath = function (path)
{
    var files = fs.readdirSync(path);
    return files;
}

var filterFilesByName = function (files, filterName)
{
    var filteredFiles = [];
    for (var i = 0; i < files.length; i++)
    {
        if(files[i].substring(0, filterName.length) == filterName)
        {
            filteredFiles.push(files[i]);
        }
    };
    return filteredFiles;
}

var filterFilesByExtension = function (files, extension)
{
    var filteredFiles = [];
    for (var i = 0; i < files.length; i++)
    {
        if(getExtension(files[i]) == extension)
        {
            var lenghtWithoutExt = files[i].length - extension.length - 1;
            filteredFiles.push(files[i].substring(0, lenghtWithoutExt));
        }
    };

    return filteredFiles;
}

var countComponents = function ()
{
    var components = getFilesFromPath(path.join(root, "src/common/")); // Busca en ABP/src/common/

    components = filterFilesByName(components, "Component");
    components = filterFilesByExtension(components, "cpp");

    var componentDiv = $(".componentList");

    componentDiv.append($("<h3>").text("Hay " + components.length + " componentes"));

    for (var i = 0; i < components.length; i++)
    {
        componentDiv.append($("<p>").text(components[i]));
    };


}

function htmlText(text) {
    var htmls = [];
    var lines = text.split(/\n/);
    // The temporary <div/> is to perform HTML entity encoding reliably.
    //
    // document.createElement() is *much* faster than jQuery('<div/>')
    // http://stackoverflow.com/questions/268490/
    //
    // You don't need jQuery but then you need to struggle with browser
    // differences in innerText/textContent yourself
    var tmpDiv = jQuery(document.createElement('div'));
    for (var i = 0 ; i < lines.length ; i++) {
        htmls.push(tmpDiv.text(lines[i]).html());
    }
    return htmls.join("<br>");
}

var generateMap = function () 
{
    
    var image = document.getElementById("map");

    var context = document.getElementById("canvas").getContext("2d");

    context.drawImage(image, 0, 0);

    var data = context.getImageData(0,0, image.width, image.height);

    var map = data.width + "\n";

    for (var i = 0; i < data.width; i++)
    {
        for (var j = 0; j < data.height; j++)
        {
            if(data.data[i * data.width * 4+ j * 4] == 255)
            {
                map += "E";
            } else
            {
                map += "W";
            }
        }
        map += "\n";
    }

    fs.writeFileSync(path.join(root, "data/map.txt"), map);

    $("#mapText").html(htmlText(map));
    console.log("click");
}



$("#generateMap").click(generateMap);
