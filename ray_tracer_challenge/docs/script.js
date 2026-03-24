function attachIntegerInput(el, { min = 0, max = 4096 } = {}) {
  el.addEventListener("input", () => {
    let n = parseInt(v, 10);
    if (!Number.isFinite(n)) {
      el.value = "";
      return;
    }
    if (n < min) n = min;
    if (n > max) n = max;
    el.value = String(n);
  });

  el.addEventListener("blur", () => {
    if (el.value === "") el.value = String(min);
  });
}

window.addEventListener("DOMContentLoaded", () => {
  attachIntegerInput(document.getElementById("width"),  { min: 1, max: 4096 });
  attachIntegerInput(document.getElementById("height"), { min: 1, max: 4096 });
});


function render() {
  
  const heightInput = document.getElementById("height");
  const widthInput  = document.getElementById("width");

  const intPattern = /^[0-9]\d*$/;
  const heightStr = heightInput.value.trim();
  const widthStr  = widthInput.value.trim();

  if (!intPattern.test(heightStr) || !intPattern.test(widthStr)) {
    alert("invalid input");
    return;
  }

  const height = Number(heightStr);
  const width  = Number(widthStr);

  // optional hard cap
  const MAX_PIXELS = 4096 * 4096;
  if (height * width > MAX_PIXELS) {
    alert("Resolution too large.");
    return;
  }

  let sc_test = null;
  let pixel_arr = null;

  const loadingContainer = document.getElementById("loading-container");

  if (loadingContainer) {
    loadingContainer.style.display = "flex";
  }

  scene_canvas_test()
    .then((Module) => {
      sc_test = Module;
      sc_test.sphere_on_plane_render_pixels = sc_test.cwrap(
        "sphere_on_plane_render_pixels",
        null,
        ["number", "number", "number"]
      );
      pixel_arr = new Uint8Array(
        sc_test.HEAPU8.buffer,
        sc_test._malloc(height * width * 4),
        height * width * 4
      );
      const canvas = document.getElementById("canvas");
      canvas.width = width;
      canvas.height = height;
      const canvas_context = canvas.getContext("2d");

      sc_test.sphere_on_plane_render_pixels(pixel_arr.byteOffset, width, height);
      const image = new ImageData(new Uint8ClampedArray(pixel_arr), width);
      canvas_context.putImageData(image, 0, 0);

      if (loadingContainer) {
        loadingContainer.style.display = "none";
      }
    })
    .catch(() => {
      if (loadingContainer) {
        loadingContainer.style.display = "none";
      }
    });
}
