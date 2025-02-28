document.addEventListener('DOMContentLoaded', function () {
    const checkbox = document.getElementById('confirmUpload');
    const simpleUploadForm = document.getElementById('simpleUploadForm');
    const advancedUploadForm = document.getElementById('advancedUploadForm');
    const simpleUploadSection = document.getElementById('simple-upload');
    const advancedUploadSection = document.getElementById('advanced-upload');

    // Toggle between simple and advanced upload modes
    checkbox.addEventListener('change', function () {
        if (checkbox.checked) {
            simpleUploadSection.classList.add('hidden');
            advancedUploadSection.classList.remove('hidden');
        } else {
            simpleUploadSection.classList.remove('hidden');
            advancedUploadSection.classList.add('hidden');
        }
    });

    // Handle simple file upload
    simpleUploadForm.addEventListener('submit', function (event) {
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

    // Handle advanced file upload
    advancedUploadForm.addEventListener('submit', function (event) {
        event.preventDefault();

        const filenameInput = document.getElementById('filenameInput');
        const contentInput = document.getElementById('contentInput');
        const filename = filenameInput.value;
        const content = contentInput.value;

        if (!filename || !content) {
            alert('Please provide both file name and content');
            return;
        }

        const blob = new Blob([content], { type: 'text/plain' });
        const file = new File([blob], filename, { type: 'text/plain' });

        const formData = new FormData();
        formData.append('file', file);

        fetch('/upload', {
            method: 'POST',
            body: formData
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert('File created and uploaded successfully');
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
