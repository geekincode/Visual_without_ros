import React from 'react'

export default function App() {
  return (
    <div className="container">
      <header>
        <h1>Visual_without_ros</h1>
        <p className="tagline">A visualization project (原仓库: geekincode/Visual_without_ros)</p>
      </header>

      <main>
        <section className="card">
          <h2>About</h2>
          <p>
            This site provides documentation and demo assets for the <strong>Visual_without_ros</strong> project.
            It is a visualization tool for visual-inertial navigation systems without ROS dependencies, 
            built with Vite + React + TypeScript and deployed to GitHub Pages.
          </p>
          <p>
            Repository: <a href="https://github.com/geekincode/Visual_without_ros" target="_blank" rel="noreferrer">geekincode/Visual_without_ros</a>
          </p>
        </section>

        <section className="card">
          <h2>Project Overview</h2>
          <p>
            Visual_without_ros is an open-source project focused on visual-inertial navigation systems 
            that operates without the traditional ROS (Robot Operating System) dependencies. 
            This approach allows for more lightweight deployment and easier integration with 
            non-ROS environments while maintaining the accuracy and reliability of visual-inertial odometry.
          </p>
          <p>
            The project likely builds upon state-of-the-art algorithms such as VINS-Mono, 
            providing visualization tools for trajectory estimation, sensor fusion, 
            and mapping in robotics applications.
          </p>
        </section>

        <section className="card">
          <h2>Features / Pages</h2>
          <ul>
            <li>Visual-inertial navigation visualization</li>
            <li>Trajectory estimation display</li>
            <li>Sensor fusion demonstration</li>
            <li>Mapping capabilities showcase</li>
            <li>Documentation and usage guides</li>
            <li>Performance benchmarks</li>
          </ul>
        </section>

        <section className="card">
          <h2>Technical Details</h2>
          <p>
            The system integrates visual and inertial measurements to provide accurate pose estimation 
            for robotics applications. Key features may include:
          </p>
          <ul>
            <li>Real-time 6-DOF pose estimation</li>
            <li>Robust tracking in dynamic environments</li>
            <li>Efficient bundle adjustment</li>
            <li>Loop closure detection</li>
            <li>Online calibration capabilities</li>
          </ul>
        </section>

        <section className="card">
          <h2>Installation & Usage</h2>
          <p>
            For detailed installation and usage instructions, please refer to the 
            <a href="https://github.com/geekincode/Visual_without_ros" target="_blank" rel="noreferrer"> GitHub repository</a>.
            The project is designed to be easy to set up and run without ROS dependencies.
          </p>
          <p>
            Typical usage might involve:
          </p>
          <ol>
            <li>Installing required dependencies</li>
            <li>Preparing input data (images, IMU data)</li>
            <li>Running the visualization tool</li>
            <li>Analyzing results</li>
          </ol>
        </section>

        <section className="card">
          <h2>Screenshots / Demo</h2>
          <p>（在此处添加截图或 gif — 把图片放到 <code>public/</code> 或 <code>src/assets</code>）</p>
          <div className="placeholder-image">
            <p>Example visualization of trajectory estimation would appear here.</p>
          </div>
        </section>

        <section className="card small">
          <h3>Deploy</h3>
          <p>Set your GitHub username in <code>package.json</code> & adjust <code>vite.config.ts</code> base if needed, then run:</p>
          <pre className="cmd">npm install; npm run deploy</pre>
        </section>
      </main>

      <footer>
        <small>Built with Vite + React + TypeScript • Deployed with gh-pages</small>
      </footer>
    </div>
  )
}