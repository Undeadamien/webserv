document.getElementById('open-dtrala').addEventListener('click', function() {
    // Crée un nouvel élément img
    var img = document.createElement('img');
    img.src = './images/rickroll.gif'; // Chemin vers ton image GIF
    img.alt = 'Rickroll';

    // Remplace le contenu de la section "Mandatory" par l'image
    var mandatorySection = document.getElementById('mandatory-section');
    mandatorySection.innerHTML = ''; // Vide le contenu actuel
    mandatorySection.appendChild(img); // Ajoute l'image
});

document.getElementById('open-eviala').addEventListener('click', function() {
    // Crée un nouvel élément img
    var img = document.createElement('img');
    img.src = './images/non-mario.gif'; // Chemin vers ton image GIF
    img.alt = 'Rickroll';

    // Remplace le contenu de la section "Mandatory" par l'image
    var mandatorySection = document.getElementById('mandatory-section');
    mandatorySection.innerHTML = ''; // Vide le contenu actuel
    mandatorySection.appendChild(img); // Ajoute l'image
});

document.getElementById('toggle-checkbox').addEventListener('change', function() {
    const mandatorySection = document.getElementById('mandatory-section');
    const bonusSection = document.getElementById('bonus-section');

    if (this.checked) {
        mandatorySection.classList.add('hidden');
        bonusSection.classList.remove('hidden');
    } else {
        mandatorySection.classList.remove('hidden');
        bonusSection.classList.add('hidden');
    }
});


/* partie del et tele */
document.addEventListener('DOMContentLoaded', function () {
    const uploadForm = document.getElementById('uploadForm');

    uploadForm.addEventListener('submit', function (event) {
        event.preventDefault();

        const fileInput = document.getElementById('fileInput');
        const file = fileInput.files[0];

        if (!file) {
            alert('Please select a file to upload');
            return;
        }

        const formData = new FormData();
        formData.append('file', file);

        fetch('/upload', {
            method: 'POST',
            body: formData
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert('File uploaded successfully');
            } else {
                alert('Failed to upload file');
            }
        })
        .catch(error => {
            console.error('Error uploading file:', error);
            alert('Failed to upload file');
        });
    });
});
