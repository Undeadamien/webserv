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
// Handle simple file upload
document.getElementById('simpleUploadForm').addEventListener('submit', function (event) {
    event.preventDefault();

    const fileInput = document.getElementById('fileInput');
    const file = fileInput.files[0];

    if (!file) {
        alert('Please select a file to upload');
        return;
    }

    const reader = new FileReader();
    reader.onload = function(e) {
        const content = e.target.result;
        const jsonData = {
            filename: file.name,
            content: content
        };

        fetch('/upload', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(jsonData)
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
    };
    reader.readAsText(file);
});

// Handle advanced file upload
document.getElementById('advancedUploadForm').addEventListener('submit', function (event) {
    event.preventDefault();

    const filenameInput = document.getElementById('filenameInput');
    const contentInput = document.getElementById('contentInput');
    const filename = filenameInput.value;
    const content = contentInput.value;

    if (!filename || !content) {
        alert('Please provide both file name and content');
        return;
    }

    const jsonData = {
        filename: filename,
        content: content
    };

    fetch('/upload', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(jsonData)
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
