import QtQuick

Canvas {
    id: graph

    property var backend
    property var samples: backend ? backend.altitudeHistory : []

    onSamplesChanged: requestPaint()

    onPaint: {
        const context = getContext("2d");
        context.reset();

        const left = 52;
        const right = width - 12;
        const top = 12;
        const bottom = height - 28;
        const plotWidth = right - left;
        const plotHeight = bottom - top;

        if (plotWidth <= 0 || plotHeight <= 0) {
            return;
        }

        context.strokeStyle = "#49605d";
        context.lineWidth = 1;
        context.fillStyle = "#9eb3aa";
        context.font = "11px sans-serif";

        for (let i = 0; i <= 4; ++i) {
            const y = top + plotHeight * i / 4;
            context.beginPath();
            context.moveTo(left, y);
            context.lineTo(right, y);
            context.stroke();
        }

        context.textAlign = "center";
        context.fillText("Time (s)", left + plotWidth / 2, height - 4);

        if (samples.length === 0) {
            return;
        }

        let minimumAltitude = samples[0].altitude;
        let maximumAltitude = minimumAltitude;
        for (let i = 1; i < samples.length; ++i) {
            minimumAltitude = Math.min(minimumAltitude, samples[i].altitude);
            maximumAltitude = Math.max(maximumAltitude, samples[i].altitude);
        }

        if (minimumAltitude === maximumAltitude) {
            minimumAltitude -= 1;
            maximumAltitude += 1;
        } else {
            const padding = (maximumAltitude - minimumAltitude) * 0.1;
            minimumAltitude -= padding;
            maximumAltitude += padding;
        }

        const latestTime = samples[samples.length - 1].time;
        const dayLength = 24 * 60 * 60 * 1000;

        context.strokeStyle = "#334744";
        context.fillStyle = "#9eb3aa";
        context.textAlign = "center";
        for (let i = 0; i <= 5; ++i) {
            const x = left + plotWidth * i / 5;
            context.beginPath();
            context.moveTo(x, top);
            context.lineTo(x, bottom);
            context.stroke();
            context.fillText(String(i - 5), x, bottom + 14);
        }

        context.textAlign = "right";
        context.fillStyle = "#9eb3aa";
        for (let i = 0; i <= 4; ++i) {
            const altitude = maximumAltitude - (maximumAltitude - minimumAltitude) * i / 4;
            const y = top + plotHeight * i / 4;
            context.fillText(altitude.toFixed(0) + " m", left - 8, y + 4);
        }

        context.beginPath();
        for (let i = 0; i < samples.length; ++i) {
            let elapsed = samples[i].time - latestTime;
            if (elapsed > 0) {
                elapsed -= dayLength;
            }
            const x = right + plotWidth * elapsed / 5000;
            const y = bottom - plotHeight * (samples[i].altitude - minimumAltitude) / (maximumAltitude
                                                                                       - minimumAltitude);

            if (i === 0) {
                context.moveTo(x, y);
            } else {
                context.lineTo(x, y);
            }
        }

        context.strokeStyle = "#75d3ad";
        context.lineWidth = 2;
        context.stroke();

        if (samples.length === 1) {
            const sampleX = right;
            const sampleY = bottom - plotHeight * (samples[0].altitude - minimumAltitude) / (maximumAltitude
                                                                                             - minimumAltitude);
            context.beginPath();
            context.arc(sampleX, sampleY, 3, 0, 2 * Math.PI);
            context.fillStyle = "#75d3ad";
            context.fill();
        }
    }
}
