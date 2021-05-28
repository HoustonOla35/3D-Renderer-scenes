document.addEventListener('DOMContentLoaded', function() {

    // Sliders
    var sliders = document.getElementsByClassName('splide');
    for (var i=0; i<sliders.length; i++) {
        new Splide(sliders[i], {
            autoplay: true,
            speed: 600,
            rewind: true
        }).mount();
    }

    // Tabs
    var tabs = document.getElementsByClassName('tabs');
    for (var i=0; i<tabs.length; i++) {
        M.Tabs.init(tabs[i], {});
    }

    // Modals
    M.Modal.init(document.querySelectorAll('.modal'), {});
});

window.MathJax = {
  tex: {
    macros: {
      d: ["\\operatorname{d}\\!{#1}", 1],
      par: ["\\left({#1}\\right)", 1]
    }
  }
};
