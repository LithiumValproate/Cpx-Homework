class StudentManager {
    constructor() {
        this.students = [];
        this.currentEditingId = null;
        this.qtBridge = null;
        this.init();
    }

    async init() {
        // 等待Qt WebChannel可用
        await this.waitForQtBridge();
        this.setupEventListeners();
        await this.loadStudents();
        this.renderStudents();
    }

    // 等待Qt Bridge可用
    waitForQtBridge() {
        return new Promise((resolve) => {
            if (typeof qt !== 'undefined' && qt.webChannelTransport) {
                new QWebChannel(qt.webChannelTransport, (channel) => {
                    this.qtBridge = channel.objects.qtBridge;
                    console.log('Qt Bridge connected');
                    resolve();
                });
            } else {
                // 如果Qt Bridge不可用，使用模拟数据
                console.warn('Qt Bridge not available, using mock data');
                this.qtBridge = this.createMockBridge();
                resolve();
            }
        });
    }

    // 创建模拟的Qt Bridge（用于调试）
    createMockBridge() {
        return {
            get_students: () => {
                return [
                    {
                        id: 2021001001,
                        name: "张三",
                        sex: "Male",
                        birthdate: { year: 2001, month: 5, day: 15 },
                        admissionYear: 2021,
                        major: "计算机科学与技术",
                        courses: ["数据结构", "算法设计", "操作系统", "数据库原理"],
                        contact: { phone: "138-1234-5678", email: "zhangsan@example.com" },
                        address: { province: "北京市", city: "海淀区" },
                        status: "Active",
                        familyMembers: [
                            {
                                name: "张建国",
                                relationship: "父亲",
                                contactInfo: { phone: "139-8765-4321", email: "" }
                            }
                        ],
                        scores: {
                            "数据结构": { score: 95, gpa: 4.0 },
                            "算法设计": { score: 88, gpa: 3.7 },
                            "操作系统": { score: 92, gpa: 3.9 },
                            "数据库原理": { score: 87, gpa: 3.6 }
                        }
                    }
                ];
            },
            add_student: (data) => console.log('Mock: Adding student', data),
            update_student: (data) => console.log('Mock: Updating student', data),
            delete_student: (id) => console.log('Mock: Deleting student', id),
            log_message: (msg) => console.log('Mock Log:', msg),
            show_notification: (title, msg) => console.log('Mock Notification:', title, msg)
        };
    }

    // 设置事件监听器
    setupEventListeners() {
        // 添加学生按钮
        document.getElementById('addStudentBtn').addEventListener('click', () => {
            this.showStudentModal();
        });

        // 导入数据按钮
        document.getElementById('importBtn').addEventListener('click', () => {
            this.importData();
        });

        // 导出数据按钮
        document.getElementById('exportBtn').addEventListener('click', () => {
            this.exportData();
        });

        // 搜索功能
        document.getElementById('searchInput').addEventListener('input', (e) => {
            this.filterStudents(e.target.value);
        });

        // 模态框关闭
        document.getElementById('closeModal').addEventListener('click', () => {
            this.closeModal();
        });

        // 保存学生信息
        document.getElementById('saveStudentBtn').addEventListener('click', () => {
            this.saveStudent();
        });

        // 添加课程
        document.getElementById('addCourseBtn').addEventListener('click', () => {
            this.addCourseField();
        });

        // 添加家庭成员
        document.getElementById('addFamilyBtn').addEventListener('click', () => {
            this.addFamilyMemberField();
        });

        // 添加成绩
        document.getElementById('addScoreBtn').addEventListener('click', () => {
            this.addScoreField();
        });

        // 点击模态框外部关闭
        document.getElementById('studentModal').addEventListener('click', (e) => {
            if (e.target.id === 'studentModal') {
                this.closeModal();
            }
        });
    }

    // 从Qt后端加载学生数据
    async loadStudents() {
        try {
            this.students = this.qtBridge.get_students();
            this.qtBridge.log_message('Students loaded from backend');
        } catch (error) {
            console.error('Error loading students:', error);
            this.qtBridge.log_message('Error loading students: ' + error.message);
        }
    }

    // 渲染学生列表
    renderStudents(studentsToRender = this.students) {
        const container = document.getElementById('studentsContainer');
        container.innerHTML = '';

        if (studentsToRender.length === 0) {
            container.innerHTML = `
                <div class="no-students">
                    <h3>暂无学生数据</h3>
                    <p>点击"添加学生"按钮开始添加学生信息</p>
                </div>
            `;
            return;
        }

        studentsToRender.forEach(student => {
            const studentCard = this.createStudentCard(student);
            container.appendChild(studentCard);
        });
    }

    // 创建学生卡片
    createStudentCard(student) {
        const card = document.createElement('div');
        card.className = 'student-card';
        card.innerHTML = `
            <div class="student-header">
                <div class="student-avatar">
                    <img src="https://via.placeholder.com/80x80/4f46e5/ffffff?text=${encodeURIComponent(student.name.charAt(0))}" alt="学生头像">
                </div>
                <div class="student-basic-info">
                    <h2 class="student-name">${student.name}</h2>
                    <div class="student-id">学号: ${student.id}</div>
                    <div class="status-badge ${this.getStatusClass(student.status)}">${this.getStatusText(student.status)}</div>
                </div>
                <div class="student-actions">
                    <button class="btn-icon edit" onclick="studentManager.editStudent(${student.id})" title="编辑">✏️</button>
                    <button class="btn-icon delete" onclick="studentManager.deleteStudent(${student.id})" title="删除">🗑️</button>
                </div>
            </div>
            <div class="student-details">
                ${this.renderBasicInfo(student)}
                ${this.renderContactInfo(student)}
                ${this.renderCoursesInfo(student)}
                ${this.renderScoresInfo(student)}
                ${this.renderFamilyInfo(student)}
            </div>
        `;
        return card;
    }

    // 渲染基本信息
    renderBasicInfo(student) {
        return `
            <div class="info-section">
                <h3>基本信息</h3>
                <div class="info-grid">
                    <div class="info-item">
                        <label>性别</label>
                        <span>${student.sex === 'Male' ? '男' : '女'}</span>
                    </div>
                    <div class="info-item">
                        <label>年龄</label>
                        <span>${this.calculateAge(student.birthdate)}岁</span>
                    </div>
                    <div class="info-item">
                        <label>生日</label>
                        <span>${student.birthdate.year}-${student.birthdate.month.toString().padStart(2, '0')}-${student.birthdate.day.toString().padStart(2, '0')}</span>
                    </div>
                    <div class="info-item">
                        <label>入学年份</label>
                        <span>${student.admissionYear}</span>
                    </div>
                    <div class="info-item">
                        <label>专业</label>
                        <span>${student.major}</span>
                    </div>
                </div>
            </div>
        `;
    }

    // 渲染联系信息
    renderContactInfo(student) {
        return `
            <div class="info-section">
                <h3>联系方式</h3>
                <div class="info-grid">
                    <div class="info-item">
                        <label>电话</label>
                        <span>${student.contact.phone}</span>
                    </div>
                    <div class="info-item">
                        <label>邮箱</label>
                        <span>${student.contact.email}</span>
                    </div>
                    <div class="info-item">
                        <label>地址</label>
                        <span>${student.address.province} ${student.address.city}</span>
                    </div>
                </div>
            </div>
        `;
    }

    // 渲染课程信息
    renderCoursesInfo(student) {
        if (!student.courses || student.courses.length === 0) {
            return `
                <div class="info-section">
                    <h3>课程信息</h3>
                    <div class="courses-container">
                        <span class="no-data">暂无课程信息</span>
                    </div>
                </div>
            `;
        }

        const courseTags = student.courses.map(course =>
            `<div class="course-tag">${course}</div>`
        ).join('');

        return `
            <div class="info-section">
                <h3>课程信息</h3>
                <div class="courses-container">
                    ${courseTags}
                </div>
            </div>
        `;
    }

    // 渲染成绩信息
    renderScoresInfo(student) {
        if (!student.scores || Object.keys(student.scores).length === 0) {
            return `
                <div class="info-section">
                    <h3>成绩信息</h3>
                    <div class="scores-container">
                        <span class="no-data">暂无成绩信息</span>
                    </div>
                </div>
            `;
        }

        const scoreItems = Object.entries(student.scores).map(([course, scoreData]) => `
            <div class="score-item">
                <div class="course-name">${course}</div>
                <div class="score-details">
                    <span class="score">${scoreData.score}分</span>
                    <span class="gpa">GPA: ${scoreData.gpa}</span>
                </div>
            </div>
        `).join('');

        const average = this.calculateAverage(student.scores);

        return `
            <div class="info-section">
                <h3>成绩信息</h3>
                <div class="scores-container">
                    ${scoreItems}
                </div>
                <div class="average-score">
                    <strong>平均分: ${average.toFixed(1)}</strong>
                </div>
            </div>
        `;
    }

    // 渲染家庭成员信息
    renderFamilyInfo(student) {
        if (!student.familyMembers || student.familyMembers.length === 0) {
            return `
                <div class="info-section">
                    <h3>家庭成员</h3>
                    <div class="family-members">
                        <span class="no-data">暂无家庭成员信息</span>
                    </div>
                </div>
            `;
        }

        const familyItems = student.familyMembers.map(member => `
            <div class="family-member">
                <div class="member-info">
                    <span class="member-name">${member.name}</span>
                    <span class="member-relationship">${member.relationship}</span>
                </div>
                <div class="member-contact">
                    <span>${member.contactInfo.phone}</span>
                </div>
            </div>
        `).join('');

        return `
            <div class="info-section">
                <h3>家庭成员</h3>
                <div class="family-members">
                    ${familyItems}
                </div>
            </div>
        `;
    }

    // 显示学生模态框
    showStudentModal(student = null) {
        const modal = document.getElementById('studentModal');
        const form = document.getElementById('studentForm');

        this.currentEditingId = student ? student.id : null;

        if (student) {
            this.populateForm(student);
            document.getElementById('modalTitle').textContent = '编辑学生信息';
        } else {
            form.reset();
            this.clearDynamicFields();
            document.getElementById('modalTitle').textContent = '添加学生信息';
        }

        modal.style.display = 'block';
    }

    // 填充表单
    populateForm(student) {
        document.getElementById('studentId').value = student.id;
        document.getElementById('studentName').value = student.name;
        document.getElementById('studentSex').value = student.sex;
        document.getElementById('birthYear').value = student.birthdate.year;
        document.getElementById('birthMonth').value = student.birthdate.month;
        document.getElementById('birthDay').value = student.birthdate.day;
        document.getElementById('admissionYear').value = student.admissionYear;
        document.getElementById('major').value = student.major;
        document.getElementById('phone').value = student.contact.phone;
        document.getElementById('email').value = student.contact.email;
        document.getElementById('province').value = student.address.province;
        document.getElementById('city').value = student.address.city;
        document.getElementById('status').value = student.status;

        // 填充课程
        this.clearDynamicFields();
        if (student.courses) {
            student.courses.forEach(course => {
                this.addCourseField(course);
            });
        }

        // 填充家庭成员
        if (student.familyMembers) {
            student.familyMembers.forEach(member => {
                this.addFamilyMemberField(member);
            });
        }

        // 填充成绩
        if (student.scores) {
            Object.entries(student.scores).forEach(([course, scoreData]) => {
                this.addScoreField(course, scoreData.score, scoreData.gpa);
            });
        }
    }

    // 清除动态字段
    clearDynamicFields() {
        document.getElementById('coursesContainer').innerHTML = '';
        document.getElementById('familyContainer').innerHTML = '';
        document.getElementById('scoresContainer').innerHTML = '';
    }

    // 添加课程字段
    addCourseField(value = '') {
        const container = document.getElementById('coursesContainer');
        const div = document.createElement('div');
        div.className = 'course-field';
        div.innerHTML = `
            <input type="text" class="course-input" value="${value}" placeholder="课程名称">
            <button type="button" class="remove-btn" onclick="this.parentElement.remove()">删除</button>
        `;
        container.appendChild(div);
    }

    // 添加家庭成员字段
    addFamilyMemberField(member = null) {
        const container = document.getElementById('familyContainer');
        const div = document.createElement('div');
        div.className = 'family-field';
        div.innerHTML = `
            <input type="text" class="family-name" value="${member ? member.name : ''}" placeholder="姓名">
            <input type="text" class="family-relationship" value="${member ? member.relationship : ''}" placeholder="关系">
            <input type="text" class="family-phone" value="${member ? member.contactInfo.phone : ''}" placeholder="电话">
            <button type="button" class="remove-btn" onclick="this.parentElement.remove()">删除</button>
        `;
        container.appendChild(div);
    }

    // 添加成绩字段
    addScoreField(course = '', score = '', gpa = '') {
        const container = document.getElementById('scoresContainer');
        const div = document.createElement('div');
        div.className = 'score-field';
        div.innerHTML = `
            <input type="text" class="score-course" value="${course}" placeholder="课程名称">
            <input type="number" class="score-value" value="${score}" placeholder="分数" min="0" max="100" step="0.1">
            <input type="number" class="score-gpa" value="${gpa}" placeholder="GPA" min="0" max="4" step="0.1">
            <button type="button" class="remove-btn" onclick="this.parentElement.remove()">删除</button>
        `;
        container.appendChild(div);
    }

    // 保存学生信息
    saveStudent() {
        const studentData = this.collectFormData();

        if (!this.validateStudentData(studentData)) {
            return;
        }

        try {
            if (this.currentEditingId) {
                // 更新学生
                this.qtBridge.update_student(studentData);
                const index = this.students.findIndex(s => s.id === this.currentEditingId);
                if (index !== -1) {
                    this.students[index] = studentData;
                }
            } else {
                // 添加新学生
                this.qtBridge.add_student(studentData);
                this.students.push(studentData);
            }

            this.closeModal();
            this.renderStudents();
            this.qtBridge.show_notification('成功', this.currentEditingId ? '学生信息已更新' : '学生信息已添加');
        } catch (error) {
            console.error('Error saving student:', error);
            this.qtBridge.log_message('Error saving student: ' + error.message);
            alert('保存失败：' + error.message);
        }
    }

    // 收集表单数据
    collectFormData() {
        const courses = Array.from(document.querySelectorAll('.course-input'))
            .map(input => input.value.trim())
            .filter(course => course);

        const familyMembers = Array.from(document.querySelectorAll('.family-field')).map(field => ({
            name: field.querySelector('.family-name').value.trim(),
            relationship: field.querySelector('.family-relationship').value.trim(),
            contactInfo: {
                phone: field.querySelector('.family-phone').value.trim(),
                email: ''
            }
        })).filter(member => member.name && member.relationship);

        const scores = {};
        Array.from(document.querySelectorAll('.score-field')).forEach(field => {
            const course = field.querySelector('.score-course').value.trim();
            const score = parseFloat(field.querySelector('.score-value').value) || 0;
            const gpa = parseFloat(field.querySelector('.score-gpa').value) || 0;
            if (course) {
                scores[course] = { score, gpa };
            }
        });

        return {
            id: parseInt(document.getElementById('studentId').value) || Date.now(),
            name: document.getElementById('studentName').value.trim(),
            sex: document.getElementById('studentSex').value,
            birthdate: {
                year: parseInt(document.getElementById('birthYear').value) || new Date().getFullYear() - 20,
                month: parseInt(document.getElementById('birthMonth').value) || 1,
                day: parseInt(document.getElementById('birthDay').value) || 1
            },
            admissionYear: parseInt(document.getElementById('admissionYear').value) || new Date().getFullYear(),
            major: document.getElementById('major').value.trim(),
            courses,
            contact: {
                phone: document.getElementById('phone').value.trim(),
                email: document.getElementById('email').value.trim()
            },
            address: {
                province: document.getElementById('province').value.trim(),
                city: document.getElementById('city').value.trim()
            },
            status: document.getElementById('status').value,
            familyMembers,
            scores
        };
    }

    // 验证学生数据
    validateStudentData(data) {
        if (!data.name) {
            alert('请输入学生姓名');
            return false;
        }
        if (!data.major) {
            alert('请输入专业');
            return false;
        }
        if (!data.contact.phone) {
            alert('请输入电话号码');
            return false;
        }
        return true;
    }

    // 编辑学生
    editStudent(studentId) {
        const student = this.students.find(s => s.id === studentId);
        if (student) {
            this.showStudentModal(student);
        }
    }

    // 删除学生
    deleteStudent(studentId) {
        if (confirm('确定要删除这个学生吗？')) {
            try {
                this.qtBridge.delete_student(studentId);
                this.students = this.students.filter(s => s.id !== studentId);
                this.renderStudents();
                this.qtBridge.show_notification('成功', '学生已删除');
            } catch (error) {
                console.error('Error deleting student:', error);
                this.qtBridge.log_message('Error deleting student: ' + error.message);
            }
        }
    }

    // 关闭模态框
    closeModal() {
        document.getElementById('studentModal').style.display = 'none';
        this.currentEditingId = null;
    }

    // 过滤学生
    filterStudents(searchTerm) {
        const filtered = this.students.filter(student =>
            student.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
            student.id.toString().includes(searchTerm) ||
            student.major.toLowerCase().includes(searchTerm.toLowerCase())
        );
        this.renderStudents(filtered);
    }

    // 导入数据
    importData() {
        this.qtBridge.open_file_dialog('导入学生数据', 'JSON Files (*.json)');
    }

    // 导出数据
    exportData() {
        this.qtBridge.save_file_dialog('导出学生数据', 'JSON Files (*.json)');
    }

    // 工具方法
    getStatusClass(status) {
        switch(status) {
            case 'Active': return 'status-active';
            case 'Leave': return 'status-leave';
            case 'Graduated': return 'status-graduated';
            default: return 'status-active';
        }
    }

    getStatusText(status) {
        switch(status) {
            case 'Active': return '在读';
            case 'Leave': return '休学';
            case 'Graduated': return '毕业';
            default: return '在读';
        }
    }

    calculateAge(birthdate) {
        const today = new Date();
        const birth = new Date(birthdate.year, birthdate.month - 1, birthdate.day);
        let age = today.getFullYear() - birth.getFullYear();
        const monthDiff = today.getMonth() - birth.getMonth();
        if (monthDiff < 0 || (monthDiff === 0 && today.getDate() < birth.getDate())) {
            age--;
        }
        return age;
    }

    calculateAverage(scores) {
        const values = Object.values(scores).map(s => s.score);
        return values.length > 0 ? values.reduce((a, b) => a + b, 0) / values.length : 0;
    }
}

// 初始化应用
let studentManager;
document.addEventListener('DOMContentLoaded', () => {
    studentManager = new StudentManager();
});

// 全局函数（供HTML调用）
window.studentManager = null;
window.addEventListener('load', () => {
    if (!window.studentManager) {
        window.studentManager = studentManager;
    }
});