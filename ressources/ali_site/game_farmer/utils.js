function loadImage(imagePath, id, z) {
    const img = new Image();
    img.src = imagePath;
    img.onerror = function() {
        console.error(`Failed to load image into square`);
    };
    img.setAttribute('id', id);
    img.style.zIndex = z;
    return img;
}

/**
 * Load an array of images, inserting a number before the file extension in the image path.
 * For example, if imagePath is '../example/path.png', and number is 3,
 * the resulting paths will be '../example/path0.png', '../example/path1.png', '../example/path2.png'.
 *
 * @param {string} imagePath - The base image path.
 * @param {string} id - The id to set as an HTML attribute to the images.
 * @param {int} z - The Z-index value for the images.
 * @param {int} number - The number of images to load.
 * @returns {HTMLImageElement[]} An array of HTMLImageElement objects representing the loaded images.
 */
function loadImages(imagePath, id, z, number) {
    let imgs = [];
    let indexOfPoint = imagePath.lastIndexOf('.');

    for (let i = 0; i < number; i++) {
        const img = new Image();
        img.src = insertToStr(imagePath, i, indexOfPoint);
        img.onerror = function() {
            console.error(`Failed to load image into square`);
        };
        img.setAttribute('id', id);
        img.style.zIndex = z;
        imgs.push(img);
    }
    return imgs;
}

function getPercent(value, percent) {
    return (value * percent) / 100;
}

function insertToStr(strTarget, strToInsert, index) {
    return strTarget.slice(0, index) + strToInsert + strTarget.slice(index);
}

function isDigit(char) {
    return char >= '0' && char <= '9'
}