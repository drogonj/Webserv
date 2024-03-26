class Map {
    constructor(elemId) {
        this.map = document.getElementById(elemId);
        let result = getPercent(screen.width, 60);
        this.map.style.width = result - (result % globalSize) + "px";
        result = getPercent(screen.height, 60);
        this.map.style.height = result - (result % globalSize) + "px";

        this.squaresPerRow = Math.floor(this.map.clientWidth / globalSize);
        this.numRows = Math.floor(this.map.clientHeight / globalSize);

        this.mapGenerator();

        this.map.addEventListener('mousedown', mouseDownEvent);
        this.map.addEventListener('mousemove', mouseMoveEvent);
    }

    addSquare(square) {
        this.map.appendChild(square);
    }

    mapGenerator() {
        for (let x = 0; x < this.numRows; x++) {
            for (let y = 0; y < this.squaresPerRow; y++) {
                const square = document.createElement('div');
                square.classList.add('square');
                if (this.#isCorner(x, y)) {
                    square.appendChild(loadImage("../image/grass_corner.png", "corner", 0))
                }
                else if (this.isBorderOfMap(x, y)) {
                    square.appendChild(loadImage("../image/grass_side.png", "side", 0))
                }
                else {
                    square.appendChild(loadImage("../image/grass.png", "ground", 0));
                }
                const img = square.querySelector('img');
                img.style.transform = this.#rotateCalculation(x, y);
                this.addSquare(square);
            }
        }
    }

    #isCorner(x, y) {
        return x === 0 && y === 0 || x === 0 && y === (this.squaresPerRow - 1) || y === 0 && x === (this.numRows - 1) || x === (this.numRows - 1) && y === (this.squaresPerRow - 1)
    }

    isBorderOfMap(x, y) {
        return x === 0 || y === 0 || x === (this.numRows - 1) || y === (this.squaresPerRow - 1)
    }

    #rotateCalculation(x, y) {
        if (x === 0 && y > 0)
            return "rotate(90deg)";
        else if (y === (this.squaresPerRow - 1))
            return "rotate(180deg)";
        else if (x === (this.numRows - 1))
            return "rotate(270deg)";
        return "rotate(0deg)";
    }

    plantCrop(block, x, y) {
        let row = Math.floor(y / globalSize);
        let column = Math.floor(x / globalSize);
        if (map.isBorderOfMap(row, column))
            return;
        const index = row * map.squaresPerRow + column;
        const square = document.querySelectorAll('.square')[index];

        if (square.querySelector('#crops'))
            return;
        square.appendChild(block.images[0].cloneNode(true));
        for (let i= 1; i <= block.images.length - 1; i++) {
            setTimeout(function() {
                square.removeChild(square.querySelector('#crops'));
                square.appendChild(block.images[i].cloneNode(true));
            }, block.timeToGrowth * i + (Math.random() * block.timeToGrowth));
        }
    }
}

function mouseDownEvent(event) {
    const mapRect = map.map.getBoundingClientRect();
    const x = event.clientX - mapRect.left;
    const y = event.clientY - mapRect.top;
    map.plantCrop(handBlock, x, y);
}

function mouseMoveEvent(event) {

}