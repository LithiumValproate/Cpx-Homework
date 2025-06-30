// 全局变量
let students = [];
let currentPage = 1;
let pageSize = 10;
let filteredStudents = [];
let editingStudentId = null;
let deleteStudentId = null;

// 初始化应用
document.addEventListener('DOMContentLoaded', function() {
    loadStudents();
    loadSampleData();
    updateStudentTable();
    updateStatistics();
    setupEventListeners();
});

// 设置事件监听器
function setupEventListeners() {
    // 搜索输入框实时搜索
    document.getElementById('searchInput').addEventListener('input', function() {
        searchStudents();
    });
    
    // 表单验证
    const form = document.getElementById('studentForm');
    const inputs = form.querySelectorAll('input');
    inputs.forEach(input => {
        input.addEventListener('blur', validateField);
        input.addEventListener('input', clearError);
    });
    
    // 点击模态框外部关闭
    document.getElementById('studentModal').addEventListener('click', function(e) {
        if (e.target === this) {
            closeModal();
        }
    });
    
    document.getElementById('confirmModal').addEventListener('click', function(e) {
        if (e.target === this) {
            closeConfirmModal();
        }
    });
}

// 加载示例数据
function loadSampleData() {
    if (students.length === 0) {
        const sampleStudents = [
            {
                id: '2021001',
                name: '张三',
                age: 20,
                major: '计算机科学与技术',
                className: '计科21-1',
                enrollDate: '2021-09-01'
            },
            {
                id: '2021002',
                name: '李四',
                age: 19,
                major: '软件工程',
                className: '软工21-1',
                enrollDate: '2021-09-01'
            },
            {
                id: '2021003',
                name: '王五',
                age: 21,
                major: '数据科学与大数据技术',
                className: '数科21-1',
                enrollDate: '2021-09-01'
            },
            {
                id: '2021004',
                name: '赵六',
                age: 20,
                major: '人工智能',
                className: 'AI21-1',
                enrollDate: '2021-09-01'
            },
            {
                id: '2021005',
                name: '钱七',
                age: 19,
                major: '网络工程',
                className: '网工21-1',
                enrollDate: '2021-09-01'
            }
        ];
        
        students = sampleStudents;
        saveStudents();
    }
}

// 本地存储操作
function saveStudents() {
    localStorage.setItem('students', JSON.stringify(students));
}

function loadStudents() {
    const saved = localStorage.getItem('students');
    if (saved) {
        students = JSON.parse(saved);
    }
}

// 页面切换
function showSection(sectionName) {
    // 隐藏所有section
    document.querySelectorAll('.section').forEach(section => {
        section.classList.remove('active');
    });
    
    // 显示指定section
    document.getElementById(sectionName + '-section').classList.add('active');
    
    // 更新侧边栏active状态
    document.querySelectorAll('.sidebar-link').forEach(link => {
        link.classList.remove('active');
    });
    
    event.target.closest('.sidebar-link').classList.add('active');
    
    // 如果是统计页面，更新统计数据
    if (sectionName === 'statistics') {
        updateStatistics();
    }
}

// 更新学生表格
function updateStudentTable() {
    const tbody = document.getElementById('studentTableBody');
    const emptyState = document.getElementById('emptyState');
    
    // 获取当前页的数据
    const startIndex = (currentPage - 1) * pageSize;
    const endIndex = startIndex + pageSize;
    const currentStudents = filteredStudents.slice(startIndex, endIndex);
    
    // 清空表格
    tbody.innerHTML = '';
    
    if (currentStudents.length === 0) {
        emptyState.style.display = 'block';
        document.querySelector('.student-table').style.display = 'none';
    } else {
        emptyState.style.display = 'none';
        document.querySelector('.student-table').style.display = 'table';
        
        currentStudents.forEach(student => {
            const row = createStudentRow(student);
            tbody.appendChild(row);
        });
    }
    
    updatePagination();
}

// 创建学生行
function createStudentRow(student) {
    const row = document.createElement('tr');
    row.innerHTML = `
        <td>${student.id}</td>
        <td>${student.name}</td>
        <td>${student.age}</td>
        <td>${student.major}</td>
        <td>${student.className}</td>
        <td>${student.enrollDate}</td>
        <td>
            <button class="btn btn-sm btn-primary" onclick="editStudent('${student.id}')">
                <i class="fas fa-edit"></i>
                编辑
            </button>
            <button class="btn btn-sm btn-danger" onclick="deleteStudent('${student.id}')">
                <i class="fas fa-trash"></i>
                删除
            </button>
        </td>
    `;
    return row;
}

// 更新分页
function updatePagination() {
    const pagination = document.getElementById('pagination');
    const totalPages = Math.ceil(filteredStudents.length / pageSize);
    
    pagination.innerHTML = '';
    
    if (totalPages <= 1) return;
    
    // 上一页按钮
    const prevBtn = createPageButton('上一页', currentPage - 1, currentPage === 1);
    pagination.appendChild(prevBtn);
    
    // 页码按钮
    for (let i = 1; i <= totalPages; i++) {
        if (i === 1 || i === totalPages || Math.abs(i - currentPage) <= 2) {
            const pageBtn = createPageButton(i, i, false, i === currentPage);
            pagination.appendChild(pageBtn);
        } else if (i === currentPage - 3 || i === currentPage + 3) {
            const ellipsis = document.createElement('span');
            ellipsis.textContent = '...';
            ellipsis.className = 'page-ellipsis';
            pagination.appendChild(ellipsis);
        }
    }
    
    // 下一页按钮
    const nextBtn = createPageButton('下一页', currentPage + 1, currentPage === totalPages);
    pagination.appendChild(nextBtn);
}

// 创建分页按钮
function createPageButton(text, page, disabled, active = false) {
    const button = document.createElement('button');
    button.textContent = text;
    button.className = `page-btn ${active ? 'active' : ''}`;
    button.disabled = disabled;
    
    if (!disabled) {
        button.onclick = () => {
            currentPage = page;
            updateStudentTable();
        };
    }
    
    return button;
}

// 搜索学生
function searchStudents() {
    const query = document.getElementById('searchInput').value.toLowerCase().trim();
    
    if (!query) {
        filteredStudents = [...students];
    } else {
        filteredStudents = students.filter(student => 
            student.name.toLowerCase().includes(query) ||
            student.id.toLowerCase().includes(query) ||
            student.major.toLowerCase().includes(query) ||
            student.className.toLowerCase().includes(query)
        );
    }
    
    currentPage = 1;
    updateStudentTable();
}

// 清除搜索
function clearSearch() {
    document.getElementById('searchInput').value = '';
    filteredStudents = [...students];
    currentPage = 1;
    updateStudentTable();
}

// 打开添加模态框
function openAddModal() {
    editingStudentId = null;
    document.getElementById('modalTitle').textContent = '添加学生';
    document.getElementById('studentForm').reset();
    document.getElementById('enrollDate').value = new Date().toISOString().split('T')[0];
    clearFormErrors();
    openModal();
}

// 编辑学生
function editStudent(studentId) {
    const student = students.find(s => s.id === studentId);
    if (!student) return;
    
    editingStudentId = studentId;
    document.getElementById('modalTitle').textContent = '编辑学生信息';
    
    // 填充表单
    document.getElementById('studentId').value = student.id;
    document.getElementById('studentName').value = student.name;
    document.getElementById('studentAge').value = student.age;
    document.getElementById('studentMajor').value = student.major;
    document.getElementById('studentClass').value = student.className;
    document.getElementById('enrollDate').value = student.enrollDate;
    
    clearFormErrors();
    openModal();
}

// 删除学生
function deleteStudent(studentId) {
    deleteStudentId = studentId;
    document.getElementById('confirmModal').classList.add('active');
}

// 确认删除
function confirmDelete() {
    if (deleteStudentId) {
        students = students.filter(s => s.id !== deleteStudentId);
        saveStudents();
        filteredStudents = [...students];
        updateStudentTable();
        updateStatistics();
        showToast('学生删除成功');
        closeConfirmModal();
    }
}

// 保存学生
function saveStudent() {
    if (!validateForm()) {
        return;
    }
    
    const formData = new FormData(document.getElementById('studentForm'));
    const studentData = {
        id: formData.get('studentId').trim(),
        name: formData.get('studentName').trim(),
        age: parseInt(formData.get('studentAge')),
        major: formData.get('studentMajor').trim(),
        className: formData.get('studentClass').trim(),
        enrollDate: formData.get('enrollDate')
    };
    
    // 检查学号是否重复
    const existingStudent = students.find(s => s.id === studentData.id && s.id !== editingStudentId);
    if (existingStudent) {
        showError('studentId', '学号已存在');
        return;
    }
    
    if (editingStudentId) {
        // 编辑模式
        const index = students.findIndex(s => s.id === editingStudentId);
        if (index !== -1) {
            students[index] = studentData;
        }
        showToast('学生信息更新成功');
    } else {
        // 添加模式
        students.push(studentData);
        showToast('学生添加成功');
    }
    
    saveStudents();
    filteredStudents = [...students];
    updateStudentTable();
    updateStatistics();
    closeModal();
}

// 表单验证
function validateForm() {
    let isValid = true;
    
    // 验证学号
    const studentId = document.getElementById('studentId').value.trim();
    if (!studentId) {
        showError('studentId', '学号不能为空');
        isValid = false;
    } else if (!/^\d{7}$/.test(studentId)) {
        showError('studentId', '学号应为7位数字');
        isValid = false;
    }
    
    // 验证姓名
    const studentName = document.getElementById('studentName').value.trim();
    if (!studentName) {
        showError('studentName', '姓名不能为空');
        isValid = false;
    } else if (studentName.length < 2 || studentName.length > 10) {
        showError('studentName', '姓名长度应在2-10个字符之间');
        isValid = false;
    }
    
    // 验证年龄
    const studentAge = parseInt(document.getElementById('studentAge').value);
    if (!studentAge || studentAge < 16 || studentAge > 35) {
        showError('studentAge', '年龄应在16-35之间');
        isValid = false;
    }
    
    // 验证专业
    const studentMajor = document.getElementById('studentMajor').value.trim();
    if (!studentMajor) {
        showError('studentMajor', '专业不能为空');
        isValid = false;
    }
    
    // 验证班级
    const studentClass = document.getElementById('studentClass').value.trim();
    if (!studentClass) {
        showError('studentClass', '班级不能为空');
        isValid = false;
    }
    
    // 验证入学日期
    const enrollDate = document.getElementById('enrollDate').value;
    if (!enrollDate) {
        showError('enrollDate', '入学日期不能为空');
        isValid = false;
    }
    
    return isValid;
}

// 单个字段验证
function validateField(event) {
    const field = event.target;
    const fieldName = field.name;
    const value = field.value.trim();
    
    clearError(event);
    
    switch (fieldName) {
        case 'studentId':
            if (!value) {
                showError('studentId', '学号不能为空');
            } else if (!/^\d{7}$/.test(value)) {
                showError('studentId', '学号应为7位数字');
            }
            break;
        case 'studentName':
            if (!value) {
                showError('studentName', '姓名不能为空');
            } else if (value.length < 2 || value.length > 10) {
                showError('studentName', '姓名长度应在2-10个字符之间');
            }
            break;
        case 'studentAge':
            const age = parseInt(value);
            if (!age || age < 16 || age > 35) {
                showError('studentAge', '年龄应在16-35之间');
            }
            break;
        case 'studentMajor':
            if (!value) {
                showError('studentMajor', '专业不能为空');
            }
            break;
        case 'studentClass':
            if (!value) {
                showError('studentClass', '班级不能为空');
            }
            break;
        case 'enrollDate':
            if (!value) {
                showError('enrollDate', '入学日期不能为空');
            }
            break;
    }
}

// 显示错误信息
function showError(fieldName, message) {
    const errorElement = document.getElementById(fieldName + 'Error');
    if (errorElement) {
        errorElement.textContent = message;
        errorElement.classList.add('active');
    }
}

// 清除错误信息
function clearError(event) {
    const fieldName = event.target.name;
    const errorElement = document.getElementById(fieldName + 'Error');
    if (errorElement) {
        errorElement.classList.remove('active');
    }
}

// 清除所有表单错误
function clearFormErrors() {
    document.querySelectorAll('.error-message').forEach(error => {
        error.classList.remove('active');
    });
}

// 模态框操作
function openModal() {
    document.getElementById('studentModal').classList.add('active');
    document.body.style.overflow = 'hidden';
}

function closeModal() {
    document.getElementById('studentModal').classList.remove('active');
    document.body.style.overflow = 'auto';
    editingStudentId = null;
}

function closeConfirmModal() {
    document.getElementById('confirmModal').classList.remove('active');
    deleteStudentId = null;
}

// 更新统计数据
function updateStatistics() {
    // 总学生数
    document.getElementById('totalStudents').textContent = students.length;
    
    // 专业数量
    const majors = [...new Set(students.map(s => s.major))];
    document.getElementById('totalMajors').textContent = majors.length;
    
    // 平均年龄
    const avgAge = students.length > 0 ? 
        (students.reduce((sum, s) => sum + s.age, 0) / students.length).toFixed(1) : 0;
    document.getElementById('avgAge').textContent = avgAge;
    
    // 班级数量
    const classes = [...new Set(students.map(s => s.className))];
    document.getElementById('totalClasses').textContent = classes.length;
    
    // 更新专业分布图表
    updateMajorChart(majors);
}

// 更新专业分布图表
function updateMajorChart(majors) {
    const chartContainer = document.getElementById('majorChart');
    chartContainer.innerHTML = '';
    
    if (majors.length === 0) {
        chartContainer.innerHTML = '<p style="text-align: center; color: #6c757d;">暂无数据</p>';
        return;
    }
    
    const majorCounts = majors.map(major => ({
        name: major,
        count: students.filter(s => s.major === major).length
    }));
    
    const maxCount = Math.max(...majorCounts.map(m => m.count));
    
    majorCounts.forEach(major => {
        const chartItem = document.createElement('div');
        chartItem.className = 'chart-item';
        
        const percentage = (major.count / maxCount) * 100;
        
        chartItem.innerHTML = `
            <div class="chart-label">${major.name}</div>
            <div class="chart-bar">
                <div class="chart-fill" style="width: ${percentage}%"></div>
            </div>
            <div class="chart-value">${major.count}</div>
        `;
        
        chartContainer.appendChild(chartItem);
    });
}

// 显示数据统计
function showDashboard() {
    showSection('statistics');
}

// 导出数据
function exportData() {
    if (students.length === 0) {
        showToast('没有数据可导出');
        return;
    }
    
    const dataStr = JSON.stringify(students, null, 2);
    const dataBlob = new Blob([dataStr], {type: 'application/json'});
    
    const link = document.createElement('a');
    link.href = URL.createObjectURL(dataBlob);
    link.download = `students_${new Date().toISOString().split('T')[0]}.json`;
    link.click();
    
    showToast('数据导出成功');
}

// 导入数据
function importData() {
    document.getElementById('fileInput').click();
}

// 处理文件导入
function handleFileImport(event) {
    const file = event.target.files[0];
    if (!file) return;
    
    if (file.type !== 'application/json') {
        showToast('请选择JSON格式的文件');
        return;
    }
    
    const reader = new FileReader();
    reader.onload = function(e) {
        try {
            const importedData = JSON.parse(e.target.result);
            
            if (!Array.isArray(importedData)) {
                throw new Error('文件格式不正确');
            }
            
            // 验证数据格式
            const validData = importedData.filter(student => 
                student.id && student.name && student.age && 
                student.major && student.className && student.enrollDate
            );
            
            if (validData.length === 0) {
                throw new Error('没有有效的学生数据');
            }
            
            // 合并数据，避免重复
            const existingIds = new Set(students.map(s => s.id));
            const newStudents = validData.filter(s => !existingIds.has(s.id));
            
            students = [...students, ...newStudents];
            saveStudents();
            filteredStudents = [...students];
            updateStudentTable();
            updateStatistics();
            
            showToast(`成功导入 ${newStudents.length} 条学生数据`);
        } catch (error) {
            showToast('导入失败：' + error.message);
        }
    };
    
    reader.readAsText(file);
    event.target.value = ''; // 清空文件输入
}

// 更改每页显示数量
function changePageSize() {
    pageSize = parseInt(document.getElementById('pageSize').value);
    currentPage = 1;
    updateStudentTable();
}

// 更改主题
function changeTheme(theme) {
    // 移除所有主题类
    document.body.classList.remove('theme-pink', 'theme-blue', 'theme-green');
    
    // 添加新主题类
    if (theme !== 'default') {
        document.body.classList.add('theme-' + theme);
    }
    
    // 更新按钮状态
    document.querySelectorAll('.color-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    event.target.classList.add('active');
    
    // 保存主题设置
    localStorage.setItem('theme', theme);
}

// 清空所有数据
function clearAllData() {
    if (confirm('确定要清空所有数据吗？此操作不可恢复！')) {
        students = [];
        filteredStudents = [];
        saveStudents();
        updateStudentTable();
        updateStatistics();
        showToast('所有数据已清空');
    }
}

// 显示Toast提示
function showToast(message, type = 'success') {
    const toast = document.getElementById('toast');
    const toastMessage = document.getElementById('toastMessage');
    
    toastMessage.textContent = message;
    toast.classList.add('active');
    
    setTimeout(() => {
        toast.classList.remove('active');
    }, 3000);
}

// 初始化过滤数据
filteredStudents = [...students];

// 加载保存的主题
document.addEventListener('DOMContentLoaded', function() {
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme && savedTheme !== 'default') {
        document.body.classList.add('theme-' + savedTheme);
        // 更新按钮状态
        setTimeout(() => {
            const colorBtns = document.querySelectorAll('.color-btn');
            colorBtns.forEach(btn => btn.classList.remove('active'));
            const themeIndex = ['default', 'pink', 'blue', 'green'].indexOf(savedTheme);
            if (themeIndex !== -1 && colorBtns[themeIndex]) {
                colorBtns[themeIndex].classList.add('active');
            }
        }, 100);
    }
});

// 在script.js文件开头添加
let qtBridge = null;

// 等待Qt桥接准备完成
new QWebChannel(qt.webChannelTransport, function(channel) {
    qtBridge = channel.objects.qtBridge;
    console.log('Qt桥接已连接');
});

// 修改导出功能
function exportData() {
    if (students.length === 0) {
        showToast('没有数据可导出');
        return;
    }

    const dataStr = JSON.stringify(students, null, 2);

    if (qtBridge) {
        // 使用Qt原生文件对话框
        const filePath = qtBridge.saveFileDialog('导出学生数据', 'JSON文件 (*.json)');
        if (filePath) {
            const success = qtBridge.saveFile(filePath, dataStr);
            if (success) {
                showToast('数据导出成功');
                qtBridge.showNotification('导出完成', '学生数据已成功导出');
            } else {
                showToast('导出失败');
            }
        }
    } else {
        // 降级到Web方式
        const dataBlob = new Blob([dataStr], {type: 'application/json'});
        const link = document.createElement('a');
        link.href = URL.createObjectURL(dataBlob);
        link.download = `students_${new Date().toISOString().split('T')[0]}.json`;
        link.click();
        showToast('数据导出成功');
    }
}

// 修改导入功能
function importData() {
    if (qtBridge) {
        // 使用Qt原生文件对话框
        const filePath = qtBridge.openFileDialog('选择要导入的文件', 'JSON文件 (*.json)');
        if (filePath) {
            const content = qtBridge.loadFile(filePath);
            if (content) {
                try {
                    const importedData = JSON.parse(content);
                    // 处理导入数据的逻辑...
                    processImportedData(importedData);
                } catch (error) {
                    showToast('文件格式错误');
                }
            }
        }
    } else {
        // 降级到Web方式
        document.getElementById('fileInput').click();
    }
}

function processImportedData(importedData) {
    if (!Array.isArray(importedData)) {
        showToast('文件格式不正确');
        return;
    }

    const validData = importedData.filter(student =>
        student.id && student.name && student.age &&
        student.major && student.className && student.enrollDate
    );

    if (validData.length === 0) {
        showToast('没有有效的学生数据');
        return;
    }

    const existingIds = new Set(students.map(s => s.id));
    const newStudents = validData.filter(s => !existingIds.has(s.id));

    students = [...students, ...newStudents];
    saveStudents();
    filteredStudents = [...students];
    updateStudentTable();
    updateStatistics();

    showToast(`成功导入 ${newStudents.length} 条学生数据`);

    if (qtBridge) {
        qtBridge.showNotification('导入完成', `成功导入 ${newStudents.length} 条学生数据`);
    }
}